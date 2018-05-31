#include "Program.h"

Program::Program()
{

}


Program::Program( S32 argc, C8* argv[] )
{
    for( S32 i = 0; i < argc; i++ )
        args.push_back( string( argv[i] ) );
}


Program::~Program()
{

}


void Program::initialize()
{
    if( args.size() == 1 )
    {
        args.push_back( "/home/triold/data/" );
        args.push_back( "/home/triold/data/" );
    //  args.push_back( "D:\\.gaia_files\\" );
    //  args.push_back( "D:\\.gaia_files\\GCC\\" );
        args.push_back( "27" );
        args.push_back( "27" );
    }
}


void Program::shutdown()
{

}


void Program::process()
{
    cout << "===== Start processing =====" << endl << endl;

    const S32 X = 1000;
    const S32 Y = 1000;
    const string prefix = "rgaia2_";

    int n = stoi( args[3] );
    int N = stoi( args[4] );
    while( n <= N )
    {
        D64 t0 = (D64)clock() / CLOCKS_PER_SEC;

        cout << "Opening and reading catalog... ";
        string pathFileRead = args[1] + prefix + to_string( n ) + ".txt";
        string pathFileWrite = args[2] + prefix + to_string( n );
        CatalogGAIA2 partCat;
        if( !partCat.readFile_RaDec( pathFileRead ) )
        {
            cout << "Error!!! File not found." << endl;
            cout << "(" << pathFileRead.c_str() << ")" << endl << endl;
        }
        else
        {
            cout << "Done." << endl;
            cout << " - File: " << pathFileRead.c_str() << endl;
            cout << " - Objects = " << partCat.getObjCount() << endl;
            D64 t1 = (D64)clock() / CLOCKS_PER_SEC;
            cout << " - Runtime = " << t1 - t0 << " ( " << t1 << " )"  << endl;

            cout << "Start data processing... ";
            ////////////////////////////////////////////////////////////////////////
            matrix<F32> * DATA = partCat.CRTmatrixOfThis( Y, X );

            Fits fits;
            fits.setFile( pathFileWrite + "_data0_START.fits" );
            fits.setPtrDATA( DATA );
            fits.writeFitsFile();

            ////////////////////////////////////////////////////////////////////////
            cout << 1 << ".. ";
            matrix<F32> * interDATA = AlgMatrix::CRTdeviations_allX( DATA );
            matrix<F32> * interDATA2 = new matrix<F32>( Y, X );
            for( S32 j = 0; j < Y; j++ )
                for( S32 i = 0; i < X; i++ )
                {
                   (*interDATA2)[j][i] = (*DATA)[j][i];
                   (*interDATA2)[j][i] -= (*interDATA)[j][i];
                    if( (*interDATA2)[j][i] < 0.0f )  (*interDATA2)[j][i] = 0.0f;
                }

            fits.setFile( pathFileWrite + "_data1_DEV2.fits" );
            fits.setPtrDATA( interDATA );
            fits.writeFitsFile();

            fits.setFile( pathFileWrite + "_data1_DATA-DEV2.fits" );
            fits.setPtrDATA( interDATA2 );
            fits.writeFitsFile();

            delete interDATA;

            ////////////////////////////////////////////////////////////////////////
            cout << 2 << ".. ";
            interDATA = AlgMatrix::CRTsmooth_X( interDATA2, 200 );
            for( S32 j = 0; j < Y; j++ )
                for( S32 i = 0; i < X; i++ )
                {
                    if( (*interDATA)[j][i] != 0 )
                        (*DATA)[j][i] /= (*interDATA)[j][i];
                    else (*DATA)[j][i] = 0.0f;
                    (*interDATA)[j][i] *= 100;
                }

            fits.setFile( pathFileWrite + "_data2_SMOOTH.fits" );
            fits.setPtrDATA( interDATA );
            fits.writeFitsFile();

            fits.setFile( pathFileWrite + "_data2_DATA-SMOOTH.fits" );
            fits.setPtrDATA( DATA );
            fits.writeFitsFile();

            delete interDATA;
            delete interDATA2;

            ////////////////////////////////////////////////////////////////////////
         /* cout << 3 << ".. ";
            interDATA = normalization2( DATA );
            delete DATA;    DATA = interDATA;
            for( S32 j = 0; j < Y; j++ )
                for( S32 i = 0; i < X; i++ )
                    (*DATA)[j][i] *= (*DATA)[j][i];

            fits.setFile( PATHTOFILE + "_data3_NORM.fits" );
            fits.setDATA( DATA );
            fits.writeFitsFile(); */

            ////////////////////////////////////////////////////////////////////////
            cout << 3 << ".. ";
            interDATA = AlgMatrix::CRTfilter_justFilter( DATA, 7 );
            for( S32 j = 0; j < Y; j++ )
                for( S32 i = 0; i < X; i++ )
                {
                    (*DATA)[j][i] -= (*interDATA)[j][i];
                    if( (*DATA)[j][i] < 0.0f ) (*DATA)[j][i] = 0.0f;

                    (*interDATA)[j][i] *= 100;
                }

            fits.setFile( pathFileWrite + "_data3_FILTER.fits" );
            fits.setPtrDATA( interDATA );
            fits.writeFitsFile();

            fits.setFile( pathFileWrite + "_data3_DATA-FILTER.fits" );
            fits.setPtrDATA( DATA );
            fits.writeFitsFile();

            delete interDATA;

            ////////////////////////////////////////////////////////////////////////
            cout << 4 << ".. ";
            interDATA = AlgMatrix::CRTconvolution_withGauss( DATA, 10 );
            delete DATA; DATA = interDATA;

            fits.setFile( pathFileWrite + "_data4_DATACONVOL.fits" );
            fits.setPtrDATA( DATA );
            fits.writeFitsFile();

            cout << "Done." << endl;

            ////////////////////////////////////////////////////////////////////////
            cout << "Start creating catalogGC... ";
            CatalogMyGC partCatalogGC;
            vector<PictureObject> globularClasters;
            globularClasters = AlgMatrix::searcherPictureObjects( DATA );
            partCatalogGC.addObjects( globularClasters, n, 0, X, 0, Y,
                    partCat.minRa, partCat.maxRa, partCat.minDec, partCat.maxDec );
            partCatalogGC.writeToFile( pathFileWrite + "_CatalogCG.txt" );
            delete DATA;

            cout << "Done." << endl;
            cout << " - Globular Clasters: " << globularClasters.size() << endl;
            cout << " - File: " << pathFileWrite.c_str() << "_CatalogCG.txt" << endl;

            D64 t2 = (D64)clock() / CLOCKS_PER_SEC;
            cout << " - Runtime = " << t2 - t1 << " ( " << t2 << " )"  << endl;
            cout << "==========" << endl << endl;

            ////////////////////////////////////////////////////////////////////////
        }
        n++;
    }
    cout << endl << "All runtime = " << (D64)clock() / CLOCKS_PER_SEC << "." << endl;
    cout << "==== Process completed =====" << endl;
}
