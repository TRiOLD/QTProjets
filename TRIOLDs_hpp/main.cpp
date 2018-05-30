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

    matrix<float> * CRTmatrixOfThis( unsigned int Y, unsigned int X )
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

        matrix<float> * resData = new matrix<float>( Y, X );
        for( int i = 0; i < N; i++ )
        {
            int x = int((getObj( i )->getParam( RA ) - minRa) * scaleX);
            int y = int((getObj( i )->getParam( DEC ) - minDec) * scaleY);
            if( (unsigned int)x >= X )    x = (int)X - 1;
            if( (unsigned int)y >= Y )    y = (int)Y - 1;
            (*resData)[y][x]++;
        }

        return resData;
    }
};

matrix<float> * CRTsmooth_BicubicSpline( matrix<float> * data )
{
    int X = data->getSizeX();   int Y = data->getSizeY();
    matrix<float> * resData = new matrix<float>( Y, X );

    int splitting = 10;
    int klC = splitting + 1;
    int stepX = X / splitting;
    int stepY = Y / splitting;

    matrix<float> knots( klC, klC );
    for( int m = 0; m < klC; m++ )
        for( int n = 0; n < klC; n++ )
        {
            double knot = 0;
            const int K = 10;   int k = 0;
            for( int q = m * stepY - K/2; q < m * stepY + K/2; q++ )
                for( int p = n * stepX - K/2; p < n * stepX + K/2; p++ )
                    if( p > 0 && p < X && q > 0 && q < Y )
                    {
                        knot += ((double)(*data)[q][p] - knot) / ((double)k + 1.0);
                        k++;
                    }
            knots[m][n] = knot;
        }
/*
    float arr[][] =
    {     { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
          { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
          {-3, 3, 0, 0,-2,-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
          { 2,-2, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
          { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
          { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 },
          { 0, 0, 0, 0, 0, 0, 0, 0,-3, 3, 0, 0,-2,-1, 0, 0 },
          { 0, 0, 0, 0, 0, 0, 0, 0, 2,-2, 0, 0, 1, 1, 0, 0 },
          {-3, 0, 3, 0, 0, 0, 0, 0,-2, 0,-1, 0, 0, 0, 0, 0 },
          { 0, 0, 0, 0,-3, 0, 3, 0, 0, 0, 0, 0,-2, 0,-1, 0 },
          { 9,-9,-9, 9, 6, 3,-6,-3, 6,-6, 3,-3, 4, 2, 2, 1 },
          {-6, 6, 6,-6,-3,-3, 3, 3,-4, 4,-2, 2,-2,-2,-1,-1 },
          { 2, 0,-2, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
          { 0, 0, 0, 0, 2, 0,-2, 0, 0, 0, 0, 0, 1, 0, 1, 0 },
          {-6, 6, 6,-6,-4,-2, 4, 2,-3, 3,-3, 3,-2,-1,-2,-1 },
          { 4,-4,-4, 4, 2, 2,-2,-2, 2,-2, 2,-2, 1, 1, 1, 1 }    };
    MathMatrix Minv( arr, 16, 16 );     */

    return resData;
}

matrix<float> * CRTsmooth_X( matrix<float> * data, int coreSize )
{
    int X = data->getSizeX();   int Y = data->getSizeY();
    matrix<float> * resData = new matrix<float>( Y, X );

    const int halfL = int(coreSize / 2.0 + 0.5);

    for( int j = 0; j < Y; j++ )
        for( int i = 0; i < X; i++ )
        {
            vector<float> core( 5 * coreSize );
            int k = 0;

            for( int m = 0, q = -halfL; m < coreSize; m++, q++ )
                if( j+q >= 0 && j+q < Y )
                {
                    core[k] = (*data)[j+q][i];
                    k++;
                }
            for( int n = 0, p = -halfL; n < coreSize; n++, p++ )
                if( i+p >= 0 && i+p < X )
                {
                    core[k] = (*data)[j][i+p];
                    k++;
                }
            for( int nm = 0, pq = -halfL; nm < coreSize; nm++, pq++ )
                if( i+pq >= 0 && i+pq < X && j+pq >= 0 && j+pq < Y )
                {
                    core[k] = (*data)[j+pq][i+pq];
                    k++;
                }
            for( int nm = 0, pq = -halfL; nm < coreSize; nm++, pq++ )
                if( i+halfL-pq >= 0 && i+halfL-pq < X && j+pq >= 0 && j+pq < Y )
                {
                    core[k] = (*data)[j+pq][i+halfL-pq];
                    k++;
                }

            (*resData)[j][i] = Algorithms::average( core, k );
        }

    return resData;
}

int main()
{
    CatalogGaiaDR2 part27;
    part27.readFile( "//home//triold//data//rgaia2_2.txt" );

    cout << "Test" << endl;

    matrix<float> * DATA0 = part27.CRTmatrixOfThis( 1000, 1000 );
/*    matrix<float> * DATA1 = CRTsmooth_X( DATA0, 200 );
    matrix<float> * DATA2 = new matrix<float>( 1000, 1000 );
    for( int j = 0; j < 1000; j++ )
        for( int i = 0; i < 1000; i++ )
            (*DATA2)[j][i] = (*DATA0)[j][i] / (*DATA1)[j][i];

    Fits fits0, fits1, fits2;
    fits0.setPtrDATA( DATA0 );
    fits1.setPtrDATA( DATA1 );
    fits2.setPtrDATA( DATA2 );
    fits0.setFile( "//home//triold//data//rgaia2_2.fits" );
    fits1.setFile( "//home//triold//data//rgaia2_2_smooth.fits" );
    fits2.setFile( "//home//triold//data//rgaia2_2_data-smooth.fits" );
    fits0.writeFitsFile();
    fits1.writeFitsFile();
    fits2.writeFitsFile();*/

    delete DATA0;

    cout << "Test2" << endl;

    return 0;
}
