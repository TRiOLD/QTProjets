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
        args.push_back( "D:\\.gaia_files\\" );
        args.push_back( "D:\\.gaia_files\\GCC\\" );
        args.push_back( "27" );
        args.push_back( "27" );
    }
}

void Program::shutdown()
{

}

void Program::process()
{
    cout << "=== Start processing ===" << endl << endl;

    Catalog * gaiaGlobularClaster;

    int n = stoi( args[3] );
    int N = stoi( args[4] );
    while( n <= N )
    {
        D64 t0 = (double)clock() / 1000;

        Catalog * gaiaPixel;
        Catalog * starsIngaiaGlobularClaster;

        cout << "Opening and reading catalog... ";
        string pathgaiaFile = args[1] + "gaia2_" + to_string( n ) + ".txt";
        PRC.FILENAME = pathgaiaFile;
        PRC.PATHTOFILE = args[2];
        gaiaPixel = new Catalog();
        if( !gaiaPixel->readFile_gaiaPart( pathgaiaFile ) )
        {
            cout << "Error!!! File not found." << endl;
            return;
        }
        else cout << "Done." << endl;
        cout << " - File: " << pathgaiaFile.c_str() << endl;
        cout << " - Objects = " << gaiaPixel->getObjectCount() << endl;
        D64 t1 = (D64)clock() / 1000;
        cout << " - Runtime = " << t1 - t0 << " ( " << t1 << " )"  << endl;

        cout << "Searching globular clusters... ";
        gaiaGlobularClaster = PRC.dedectedGlobularClasters( gaiaPixel );
        cout << "Done." << endl;
        cout << " - Objects = " << gaiaGlobularClaster->getObjectCount() << endl;
        D64 t2 = (D64)clock() / 1000;
        cout << " - Runtime = " << t2 - t1 << " ( " << t2 << " )"  << endl;


 /*       cout << "Search stars in the clusters..." << endl;
        for( U32 i = 0; i < gaiaGlobularClaster->getObjectCount(); i++ )
        {
            cout << "Cluster #" << i + 1 << " of " << gaiaGlobularClaster->getObjectCount() << "... ";
            string globularClusterFile = "gaia_" + to_string( n ) + "_cl_" + to_string( i + 1 ) + ".txt";
            CelestialBody * cluster = gaiaGlobularClaster->getObject( i );
 //         starsIngaiaGlobularClaster = PRC.123( gaiaPixel, cluster );
            cout << "Done." << endl;
            cout << " - Number of objects: " << starsIngaiaGlobularClaster->getObjectCount() << endl;
            cout << " - Writing catalog to file: " << globularClusterFile << endl;
        }*/


        cout << "Writing catalog of Globular Clasters to file... ";
        string pathgaiaGCFile = args[2] + "gaiaGC_" + to_string( n ) + ".txt";
        gaiaGlobularClaster->writeToFile( pathgaiaGCFile );
        cout << "Done." << endl;
        cout << " - File: " << pathgaiaGCFile << endl;

        cout << "==========" << endl << endl;

        delete gaiaPixel;
        delete gaiaGlobularClaster;
        n++;
    }

    cout << endl << "All runtime = " << (D64)clock() / 1000 << "." << endl;
    cout << "== Process completed ===" << endl;
}

/*
void Program::process()
    {
        cout << "=== Start processing ===" << endl << endl;

        int n = stoi( args[3] );
        int N = stoi( args[4] );
        while( n <= N )
        {
            D64 t0 = (double)clock() / 1000;

            Catalog * gaiaPixel;
            Fits * fits;

            cout << "Opening and reading catalog... ";
            string pathgaiaFile = args[1] + "gaia_" + to_string( n ) + ".txt";
            gaiaPixel = new Catalog();
            if( !gaiaPixel->readFile_gaiaPart( pathgaiaFile ) )
            {
                cout << "File not found." << endl;
                cout << " - File: " << pathgaiaFile << endl;
                delete gaiaPixel;
            }
            else
            {
                cout << "Done." << endl;
                cout << " - File: " << pathgaiaFile << endl;
                cout << " - Objects = " << gaiaPixel->getObjectCount() << endl;
                D64 t1 = (D64)clock() / 1000;
                cout << " - Runtime = " << t1 - t0 << " ( " << t1 << " )"  << endl;

                cout << "Create FITS... ";
                fits = PRC.catalogueToFits( gaiaPixel );
                fits->setFile( pathgaiaFile + ".fits" );
                fits->writeFitsFile();
                cout << "Done." << endl;
                D64 t2 = (D64)clock() / 1000;
                cout << " - Runtime = " << t2 - t1 << " ( " << t2 << " )"  << endl;

                cout << "==========" << endl << endl;

                delete gaiaPixel;
                delete fits;
            }
            n++;

        }


    cout << endl << "All runtime = " << (D64)clock() / 1000 << "." << endl;
    cout << "== Process completed ===" << endl;
}*/


/* void Program::process()
 {
     cout << "=== Start processing ===" << endl << endl;

     Catalog * gaiaGlobularClaster = new Catalog();

     int n = stoi( args[3] );
     int N = stoi( args[4] );
     while( n <= N )
     {
         D64 t0 = (double)clock() / 1000;

         Catalog * gaiaPixelGC;
         cout << "Opening and reading catalog... ";
         string pathFile = args[1] + "gaiaGC_" + to_string( n ) + ".txt";
         gaiaPixelGC = new Catalog();
         if( !gaiaPixelGC->readFile( pathFile ) )
         {
             cout << "File not found." << endl;
             cout << " - File: " << pathFile << endl;
             delete gaiaPixelGC;
         }
         else
         {
             cout << "Done." << endl;
             cout << " - File: " << pathFile << endl;
             cout << " - Objects = " << gaiaPixelGC->getObjectCount() << endl;
             D64 t1 = (D64)clock() / 1000;
             cout << " - Runtime = " << t1 - t0 << " ( " << t1 << " )"  << endl;

             gaiaGlobularClaster->addObjects( gaiaPixelGC );

             cout << "==========" << endl << endl;

             delete gaiaPixelGC;
         }
         n++;

     }
     gaiaGlobularClaster->writeToFile( "gaiaGlobularClaster.txt" );

     cout << endl << "All runtime = " << (D64)clock() / 1000 << "." << endl;
     cout << "== Process completed ===" << endl;

 }*/


/*
void Program::process()
 {
     cout << "=== Start processing ===" << endl << endl;
     D64 t0 = (double)clock() / 1000;

     Catalog * gaiaGlobularClaster = new Catalog();
     cout << "Opening and reading catalog... ";
     string pathFile1 = args[1];

     if( !gaiaGlobularClaster->readFile( pathFile1 ) )
     {
         cout << "File not found." << endl;
         cout << " - File: " << pathFile1 << endl;
         delete gaiaGlobularClaster;

         return;
     }
     cout << "Done." << endl;
     D64 t1 = (double)clock() / 1000;
     cout << " - Runtime = " << t1 - t0 << " ( " << t1 << " )"  << endl;

     cout << "gaia clasteroider processing... " << endl;
     string pathFile2 = args[2];
     PRC.intrFunction( gaiaGlobularClaster, pathFile2 );


     cout << endl << "All runtime = " << (D64)clock() / 1000 << "." << endl;
     cout << "== Process completed ===" << endl;
 }
*/
