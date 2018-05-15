#include <iostream>
using namespace std;

#include "Astronomy/Fits.h"
#include "Astronomy/Сatalog.h"
#include "Astronomy/СelestialBody.h"
#include "Algorithms.h"
#include "MathMatrix.h"
#include "matrix.h"

class CatalogGaiaDR2 : public Catalog
{
public:
    virtual bool readFile( std::string fileName )
    {
        std::fstream file;
        file.open( fileName, std::ios_base::in );
        if( !file.is_open() )   return false;

        unsigned int rows = 0;
        std::string fileString;
        std::getline( file, fileString );
        unsigned int colms = Algorithms::calculateWordsInSrting( m_header );
        while( std::getline( file, fileString ) )
            rows++;

        file.close();   m_objects.resize( 100 );
        file.open( fileName, std::ios_base::in );

        std::string ID; int k = 0;
  //    while( std::getline( file, fileString ) )
        while( k < 100 )
        {
            std::getline( file, fileString );
            strings W = Algorithms::divideByWordsSrting( fileString, colms );

            ID = "OBJ" + std::to_string( k+1 );
            setupObject( UN_CelestialBodyType, ID, k );
            for( int i = 0; i < (int)colms; i++ )
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
    part27.readFile( "//home//triold//data//gaia2_27.txt" );

    cout << part27.writeToFile( "//home//triold//data//newGaia2_27.txt" ) << endl;


    return 0;
}
