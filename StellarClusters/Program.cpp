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
   //   args.push_back( "D:\\.gaia_files\\" );
   //   args.push_back( "D:\\.gaia_files\\GCC\\" );
        args.push_back( "0" );
        args.push_back( "27" );
    }
}


void Program::shutdown()
{

}


void Program::process0()    // Обработка пикселей каталога
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
            matrix<F32> * interDATA =  AlgMatrix::CRTnormalization_pow2Max( DATA );
            delete DATA;    DATA = interDATA;

            fits.setFile( pathFileWrite + "_data1_NORM.fits" );
            fits.setPtrDATA( DATA );
            fits.writeFitsFile();

            ////////////////////////////////////////////////////////////////////////
            cout << 2 << ".. ";
            interDATA = AlgMatrix::CRTdeviations_allX( DATA );
            matrix<F32> * interDATA2 = new matrix<F32>( Y, X );
            for( S32 j = 0; j < Y; j++ )
                for( S32 i = 0; i < X; i++ )
                {
                   (*interDATA2)[j][i] = (*DATA)[j][i];
                   (*interDATA2)[j][i] -= (*interDATA)[j][i];
                    if( (*interDATA2)[j][i] < 0.0f )  (*interDATA2)[j][i] = 0.0f;
                }

            fits.setFile( pathFileWrite + "_data2_DEV2.fits" );
            fits.setPtrDATA( interDATA );
            fits.writeFitsFile();

            fits.setFile( pathFileWrite + "_data2_DATA-DEV2.fits" );
            fits.setPtrDATA( interDATA2 );
            fits.writeFitsFile();

            delete interDATA;

            ////////////////////////////////////////////////////////////////////////
            cout << 3 << ".. ";
            interDATA = AlgMatrix::CRTsmooth_X( interDATA2, 200 );
            for( S32 j = 0; j < Y; j++ )
                for( S32 i = 0; i < X; i++ )
                {
                    if( (*interDATA)[j][i] != 0.0f && (*DATA)[j][i] >= 1.0f )
                        (*DATA)[j][i] /= (*interDATA)[j][i];
                    if( (*interDATA)[j][i] == 0.0f )
                        (*DATA)[j][i] = 0.0f;
                    (*interDATA)[j][i] *= 100;
                }

            fits.setFile( pathFileWrite + "_data3_SMOOTH.fits" );
            fits.setPtrDATA( interDATA );
            fits.writeFitsFile();

            fits.setFile( pathFileWrite + "_data3_DATA-SMOOTH.fits" );
            fits.setPtrDATA( DATA );
            fits.writeFitsFile();

            delete interDATA;
            delete interDATA2;


            ////////////////////////////////////////////////////////////////////////
            cout << 4 << ".. ";
            interDATA = AlgMatrix::CRTfilter_justFilter( DATA, 7 );
            for( S32 j = 0; j < Y; j++ )
                for( S32 i = 0; i < X; i++ )
                {
                    (*DATA)[j][i] -= (*interDATA)[j][i];
                    if( (*DATA)[j][i] < 0.0f ) (*DATA)[j][i] = 0.0f;

                    (*interDATA)[j][i] *= 100;
                }

            fits.setFile( pathFileWrite + "_data4_FILTER.fits" );
            fits.setPtrDATA( interDATA );
            fits.writeFitsFile();

            fits.setFile( pathFileWrite + "_data4_DATA-FILTER.fits" );
            fits.setPtrDATA( DATA );
            fits.writeFitsFile();

            delete interDATA;

            ////////////////////////////////////////////////////////////////////////
            cout << 5 << ".. ";
            interDATA = AlgMatrix::CRTnormalization_justNorm( DATA );
            delete DATA; DATA = interDATA;

            fits.setFile( pathFileWrite + "_data5_RESNORM.fits" );
            fits.setPtrDATA( DATA );
            fits.writeFitsFile();

            ////////////////////////////////////////////////////////////////////////
            cout << 6 << ".. ";
            interDATA = AlgMatrix::CRTconvolution_withGauss( DATA, 10 );
            delete DATA; DATA = interDATA;

            for( S32 j = 0; j < Y; j++ )
                for( S32 i = 0; i < X; i++ )
                {
                    if( (*DATA)[j][i] < 0.0f ) (*DATA)[j][i] = 0.0f;
                    if( (*DATA)[j][i] > 100.0f ) (*DATA)[j][i] = 100.0f;
                }

            fits.setFile( pathFileWrite + "_data6_DATACONVOL.fits" );
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
            cout << " - Globular Clasters: " << partCatalogGC.getObjCount() << endl;
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


void Program::process1()    // Сшивка всех найденных скоплений в один файл
{
    cout << "===== Start stitch processing =====" << endl << endl;

    const string prefix = "rgaia2_";
    const string postfix = "_CatalogCG.txt";

    CatalogMyGC CatalogGC;
    CatalogMyGC CatalogPartGC;

    int n = stoi( args[3] );
    int N = stoi( args[4] );

    while( n <= N )
    {
        cout << "Opening and reading catalog... ";
        string pathFileRead = args[1] + prefix + to_string( n ) + postfix;
        if( !CatalogPartGC.readFile( pathFileRead ) )
        {
            cout << "Error!!! File not found." << endl;
            cout << "(" << pathFileRead.c_str() << ")" << endl << endl;
        }
        else
        {
            cout << "Done." << endl;
            cout << " - File: " << pathFileRead.c_str() << endl;
            cout << " - Objects = " << CatalogPartGC.getObjCount() << endl;

            cout << "Pushing catalog... ";
            CatalogGC.pushBack( &CatalogPartGC );
            cout << "Done." << endl;
            cout << "==========" << endl << endl;
        }
        n++;
    }

    cout << "Fixing full catalogGC... ";
    CatalogMyGC CatalogGC_Fix;
    CatalogMyGC CatalogGC_FixDev;

    for( S32 i = 0; i < CatalogGC.getObjCount(); i++ )
    {
        S32 repeat = 0;
        D64 ra0 = CatalogGC.getObj( i )->getParam( 7 ) * 180 / PI;
        D64 dec0 = CatalogGC.getObj( i )->getParam( 8 ) * 180 / PI;
        for( S32 j = i + 1; j < CatalogGC.getObjCount(); j++ )
        {
            D64 ra = CatalogGC.getObj( j )->getParam( 7 ) * 180 / PI;
            D64 dec = CatalogGC.getObj( j )->getParam( 8 ) * 180 / PI;
            if( sqrt( pow( (ra - ra0), 2 ) + pow( (dec - dec0), 2 ) ) < 0.005 )
            {
                if( !repeat )
                    CatalogGC_FixDev.pushBack( &*CatalogGC.getObj( i ) );
                CatalogGC_FixDev.pushBack( &*CatalogGC.getObj( j ) );
                repeat++;
            }
        }
        if( !repeat )
            CatalogGC_Fix.pushBack( &*CatalogGC.getObj( i ) );
    }
    cout << "Done." << endl;

    cout << "Wreating full catalogGC... ";
    string pathFileWrite = args[2] + prefix + "FIX" + "AllGlobularClasters.txt";
    CatalogGC_Fix.writeToFile( pathFileWrite );

    cout << "Done." << endl;
    cout << " - All Globular Clasters: " << CatalogGC.getObjCount() << endl;
    cout << " - All FixGlobular Clasters: " << CatalogGC_Fix.getObjCount() << endl;
    cout << " - File: " << pathFileWrite.c_str() << endl;

    cout << endl << "All runtime = " << (D64)clock() / CLOCKS_PER_SEC << "." << endl;
    cout << "==== Process completed =====" << endl;
}


void Program::process2()    // Отделение скоплений от звезд фона
{
    cout << "===== Start stitch processing =====" << endl << endl;

    const string prefix = "rgaia2_";

    CatalogMyGC CatalogGCs;
    CatalogGAIA2 CatalogPartGaiaDR2;

    string pathCatGCs = args[1] + prefix + "FIX" + "AllGlobularClasters.txt" ;

    cout << "Opening and reading catalog GC... ";
    if( !CatalogGCs.readFile( pathCatGCs ) )
    {
        cout << "Error!!! File not found." << endl;
        cout << "(" << pathCatGCs.c_str() << endl << endl;
        return;
    }
    cout << "Done." << endl;
    cout << " - All Globular Clasters: " << CatalogGCs.getObjCount() << endl;
    cout << " - File: " << pathCatGCs.c_str() << endl;
    cout << "==========" << endl << endl;

    S32 pxlNumber = -1;

    S32 n = 0;
    while( n < CatalogGCs.getObjCount() )
    {
        cout << "Opening and reading catalog (Part)... ";

        D64 t1 = (D64)clock() / CLOCKS_PER_SEC;

        bool chPxl = pxlNumber != CatalogGCs.getObj( n )->getParam( 0 );
        pxlNumber = CatalogGCs.getObj( n )->getParam( 0 );
        S32 gcNumber = CatalogGCs.getObj( n )->getParam( 1 );

        string pathFileRead = args[1] + prefix + to_string( pxlNumber ) + ".txt";
        string pathFileWrite = args[2] + prefix + to_string( pxlNumber ) + "_" + to_string( gcNumber ) + ".txt";

        if( chPxl )
            if( !CatalogPartGaiaDR2.readFile( pathFileRead ) )
            {
                cout << "Error!!! File not found." << endl;
                cout << "(" << pathFileRead.c_str() << ")" << endl << endl;
                while( CatalogGCs.getObj( n )->getParam( 0 ) == pxlNumber ) n++;
                continue;
            }

        cout << "Done." << endl;
        cout << " - File: " << pathFileRead.c_str() << endl;
        cout << " - Objects = " << CatalogPartGaiaDR2.getObjCount() << endl;
        D64 t2 = (D64)clock() / CLOCKS_PER_SEC;
        cout << " - Runtime = " << t2 - t1 << " ( " << t2 << " )"  << endl;


        cout << "Processing & writing Globular Cluster data... ";
        Catalog * CatalogGC = Clustering::CRTcatalog_TrueGC( &CatalogPartGaiaDR2, CatalogGCs.getObj( n ) );
        CatalogGC->writeToFile( pathFileWrite );
        D64 t3 = (D64)clock() / CLOCKS_PER_SEC;
        cout << "Done." << endl;
        cout << " - File: " << pathFileWrite.c_str() << endl;
        cout << " - Objects = " << CatalogGC->getObjCount() << endl;
        cout << " - Runtime = " << t3 - t2 << " ( " << t3 << " )"  << endl;

        delete CatalogGC;
        cout << "==========" << endl << endl;

        n++;
    }

    cout << endl << "All runtime = " << (D64)clock() / CLOCKS_PER_SEC << "." << endl;
    cout << "==== Process completed =====" << endl;
}
