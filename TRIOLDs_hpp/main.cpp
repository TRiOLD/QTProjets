#include <iostream>
using namespace std;

#include "Astronomy/Fits.h"
#include "Astronomy/Сatalog.h"
#include "Astronomy/СelestialBody.h"
#include "AlgorithmsMain.h"
#include "MathMatrix.h"
#include "matrix.h"

class CatalogGaiaDR2 : public Catalog
{
public:
    virtual bool readFilee( std::string fileName )
    {
        std::fstream file;
        file.open( fileName, std::ios_base::in );
        if( !file.is_open() )   return false;

        unsigned int rows = 0;
        std::string fileString;
        while( std::getline( file, fileString ) )
            rows++;

        file.close();   m_objects.resize( 100 );
        file.open( fileName, std::ios_base::in );

        int colms = 2;

        std::string ID; int k = 0;
  //    while( std::getline( file, fileString ) )
        while( k < 100 )
        {
            std::getline( file, fileString );
            strings W = AlgorithmsMain::divideByWordsSrting( fileString, colms );

            ID = "OBJ" + std::to_string( k+1 );
            setupObject( UN_CelestialBodyType, ID, k );
            for( int i = 0; i < colms; i++ )
                m_objects[k].setParam( stod( W[i] ), i );

            k++;
        }

        file.close();
        return true;
    }
};

int main()
{
    cout << "Test" << endl;

    CatalogGaiaDR2 part27;
    part27.readFilee( "//home//triold//data//gaia2_27.txt" );

    cout << part27.writeToFile( "//home//triold//data//newGaia2_27.txt" ) << endl;


    return 0;
}
