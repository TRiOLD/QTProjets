#include "CatalogMyGC.h"

CatalogMyGC::CatalogMyGC()
{
    m_header = "Part\tN\tSizeX\tSizeY\tX\tY\tZ\tRA\tDEC\n";
}


void CatalogMyGC::addObjects( vector<PictureObject> spots, S32 numPart,
                              S32 minX, S32 maxX, S32 minY, S32 maxY,
                              D64 minA, D64 maxA, D64 minB, D64 maxB )
{
    S32 k0 = S32(m_objects.size());
    m_objects.resize( k0 + spots.size() );
    for( S32 k = 0; k < S32(spots.size()); k++ )
    {
        spots[k].calculateMinMaxDotsParams();
        S32 sizeX = spots[k].maxX - spots[k].minX;
        S32 sizeY = spots[k].maxY - spots[k].minY;
        S32 X = sizeX / 2 + spots[k].minX;
        S32 Y = sizeY / 2 + spots[k].minY;
        S32 Z = spots[k].maxZ;
        D64 Ra = X * (maxA - minA) / (maxX - minX) + minA;
        D64 Dec = Y * (maxB - minB) / (maxY - minY) + minB;

        m_objects[k0+k].setParam( numPart , 0 );
        m_objects[k0+k].setParam( k0+k + 1 , 1 );
        m_objects[k0+k].setParam( sizeX , 2 );
        m_objects[k0+k].setParam( sizeY , 3 );
        m_objects[k0+k].setParam( X , 4 );
        m_objects[k0+k].setParam( Y , 5 );
        m_objects[k0+k].setParam( Z , 6 );
        m_objects[k0+k].setParam( Ra , 7 );
        m_objects[k0+k].setParam( Dec , 8 );
    }
}


bool CatalogMyGC::readFile( string fileName )
{

}


bool CatalogMyGC::writeToFile( string fileName )
{
    if( m_objects.empty() )
        return false;

    std::fstream file;
    file.open( fileName, std::ios_base::out );
    if( !file.is_open() )   return false;

    file.write( m_header.c_str(), m_header.size() );

    unsigned int k = 0;
    while( k < m_objects.size() )
    {
        for( int i = 0; i < 7; i++ )
        {
            double value = m_objects[k].getParam( i );
            file << std::fixed << std::setprecision( 0 )
                 << value << "\t";
        }
        for( int i = 7; i < 9; i++ )
        {
            double value = m_objects[k].getParam( i );
            file << std::fixed << std::setprecision( 10 )
                 << value << "\t";
        }
        file << endl;
        k++;
    }

    return true;
}
