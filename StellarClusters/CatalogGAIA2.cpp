#include "CatalogGAIA2.h"

bool CatalogGAIA2::readFile( string fileName )
{
    fstream file;
    file.open( fileName, ios_base::in );
    if( !file.is_open() )   return false;

    //header GaiaDR2 (by Vova):
    //RA, DEC, eRA, eDEC, Plx, ePlx, muRA, eMuRa, muDEC, eMuDec,
    //sn, sigSn, gMag, bMag, rMag, Vr, eVr, Teff, Metal

    U32 rows = 0;
    string fileString;
    getline( file, fileString );
    U32 colms = 12; //Algorithms::calculateWordsInSrting( fileString );
    while( getline( file, fileString ) )
        rows++;

    file.close();   m_objects.resize( rows+1 );
    file.open( fileName, ios_base::in );

    S32 k = 0;
    while( getline( file, fileString ) )
    {
        strings W = Algorithms::divideByWordsSrting( fileString, colms );

        m_objects[k].setParam( stod( W[0] ), RA );
        m_objects[k].setParam( stod( W[1] ), DEC );
        m_objects[k].setParam( stod( W[4] ), eRA ); // Для Part`s с этой строчки += 2
        m_objects[k].setParam( stod( W[5] ), eDEC );
        m_objects[k].setParam( stod( W[6] ), PLX );
        m_objects[k].setParam( stod( W[7] ), ePLX );
        m_objects[k].setParam( stod( W[8] ), muRA );
        m_objects[k].setParam( stod( W[9] ), eMuRA );
        m_objects[k].setParam( stod( W[10] ), muDEC );
        m_objects[k].setParam( stod( W[11] ), eMuDEC );

        k++;
    }

    file.close();
    return true;
}


bool CatalogGAIA2::readFile_RaDec( string fileName )
{

    m_header = "RA\tDEC\n";
    fstream file;
    file.open( fileName, ios_base::in );
    if( !file.is_open() )   return false;

    U32 rows = 0;
    string fileString;
    getline( file, fileString );
    const U32 colms = 3;
    while( getline( file, fileString ) )
        rows++;

    file.close();   m_objects.resize( rows+1 );
    file.open( fileName, ios_base::in );

    S32 k = 0;
    while( getline( file, fileString ) )
    {
        strings W = Algorithms::divideByWordsSrting( fileString, colms );
        m_objects[k].setParam( stod( W[0] ), RA );
        m_objects[k].setParam( stod( W[1] ), DEC );
        k++;
    }

    file.close();
    return true;
}


matrix<F32> * CatalogGAIA2::CRTmatrixOfThis( S32 Y, S32 X )
{
    matrix<F32> * resData = new matrix<F32>( Y, X );

    S32 N = getObjCount();
    minRa = minDec = 6.283184;
    maxRa = maxDec = -6.283184;
    for( S32 i = 0; i < N; i++ )
    {
        D64 val1 = getObj( i )->getParam( RA );
        D64 val2 = getObj( i )->getParam( DEC );

        if( val2 < -1.6  )   cout << i << " ";

        if( val1 < minRa )  minRa = val1;
        if( val1 > maxRa )  maxRa = val1;
        if( val2 < minDec )  minDec = val2;
        if( val2 > maxDec )  maxDec = val2;
    }

    const D64 scaleX = (D64)X / (maxRa - minRa);
    const D64 scaleY = (D64)Y / (maxDec - minDec);

    for(S32 i = 0; i < N; i++ )
    {
        S32 x = S32((getObj( i )->getParam( RA ) - minRa) * scaleX);
        S32 y = S32((getObj( i )->getParam( DEC ) - minDec) * scaleY);
        if( (S32)x >= X )    x = (S32)X - 1;
        if( (S32)y >= Y )    y = (S32)Y - 1;
        (*resData)[y][x]++;
    }

    return resData;
}
