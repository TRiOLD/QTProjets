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

        //header GaiaDR2 (by Vova):
        //RA, DEC, eRA, eDEC, Plx, ePlx, muRA, eMuRa, muDEC, eMuDec,
        //sn, sigSn, gMag, bMag, rMag, Vr, eVr, Teff, Metal

        unsigned int rows = 0;
        std::string fileString;
        std::getline( file, fileString );
        unsigned int colms = Algorithms::calculateWordsInSrting( fileString );
        while( std::getline( file, fileString ) )
            rows++;

        cout << rows << endl;

        file.close();   m_objects.resize( rows+1 );
        file.open( fileName, std::ios_base::in );

        int k = 0;
        while( std::getline( file, fileString ) )
        {
            strings W = Algorithms::divideByWordsSrting( fileString, colms );

            m_objects[k].setParam( stod( W[0] ), RA );
            m_objects[k].setParam( stod( W[1] ), DEC );
            m_objects[k].setParam( stod( W[2] ), eRA );
            m_objects[k].setParam( stod( W[3] ), eDEC );
            m_objects[k].setParam( stod( W[4] ), PLX );
            m_objects[k].setParam( stod( W[5] ), ePLX );
            m_objects[k].setParam( stod( W[6] ), muRA );
            m_objects[k].setParam( stod( W[7] ), eMuRA );
            m_objects[k].setParam( stod( W[8] ), muDEC );
            m_objects[k].setParam( stod( W[9] ), eMuDEC );
            m_objects[k].setParam( stod( W[12] ), MAG );

            k++;
        }

        file.close();
        return true;
    }

    Fits * createFitsOfThis( unsigned int Y, unsigned int X )
    {
        int N = getObjCount();

        double minRa, maxRa, minDec, maxDec;
        minRa = minDec = 6.283184;
        maxRa = maxDec = -6.283184;
        for( int i = 0; i < N; i++ )
        {
            double val1 = getObj( i )->getParam( RA );
            double val2 = getObj( i )->getParam( DEC );
            if( val1 < minRa )  minRa = val1;
            if( val1 > maxRa )  maxRa = val1;
            if( val2 < minDec )  minDec = val2;
            if( val2 > maxDec )  maxDec = val2;
        }

        const double scaleX = (double)X / (maxRa - minRa);
        const double scaleY = (double)Y / (maxDec - minDec);

        matrix<float> * data = new matrix<float>( Y, X );
        for( int i = 0; i < N; i++ )
        {
            int x = int((getObj( i )->getParam( RA ) - minRa) * scaleX);
            int y = int((getObj( i )->getParam( DEC ) - minDec) * scaleY);
            if( (unsigned int)x >= X )    x = (int)X - 1;
            if( (unsigned int)y >= Y )    y = (int)Y - 1;
            (*data)[y][x]++;
        }

        Fits * fits = new Fits();
        fits->setPtrDATA( data );
        return fits;
    }
};

int main()
{
    CatalogGaiaDR2 part27;
    part27.readFile( "//home//triold//data//rgaia2_27.txt" );

    cout << "Test" << endl;

    Fits * f = part27.createFitsOfThis( 1000, 1000 );
    f->setFile( "//home//triold//data//rgaia2_27.fits" );
    f->writeFitsFile();

    cout << "Test2" << endl;

    return 0;
}
