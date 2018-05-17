#include "Fits.h"

Fits::Fits()
{
    m_file;
    m_fileName = "fts";
    m_metadataSize = 0;

    m_headerKeyWords[SIMPLE] = "SIMPLE  =";
    m_headerKeyWords[BITPIX] = "BITPIX  =";
    m_headerKeyWords[NAXIS]  = "NAXIS   =";
    m_headerKeyWords[NAXIS1] = "NAXIS1  =";
    m_headerKeyWords[NAXIS2] = "NAXIS2  =";
    m_headerKeyWords[END]    = "END      ";

    for( int i = 0; i < FitsKeyWord_MAX; i++ )
        m_headerDATA[i] = 0;

    m_DATA = NULL;

    PRC = new Processing();
}

Fits::~Fits()
{

}

bool Fits::scanfHeaderFromFile()
{
    m_file.clear();
    m_file.seekp( 0 );

    string skwTemp( 9, ' ' );
    S32 hsn = 0, k = 0;

    while( m_headerKeyWords[END] != skwTemp )
    {
        if( m_file.eof() )	return false;
        m_file.read( (C8*)skwTemp.data(), 9 );

        S32 value;
        if( skwTemp == m_headerKeyWords[SIMPLE] )
        {
            char type;
            m_file >> type;
            if( type != 'T' )	return false;
            m_headerDATA[SIMPLE] = true;
            hsn++;
        }
        if( skwTemp == m_headerKeyWords[BITPIX] )
        {
            m_file >> value;
            if( value != 16 )	return false;
            m_headerDATA[BITPIX] = value;
            hsn++;
        }
        if( skwTemp == m_headerKeyWords[NAXIS] )
        {
            m_file >> value;
            if( value != 2 )	return false;
            m_headerDATA[NAXIS] = value;
            hsn++;
        }
        if( skwTemp == m_headerKeyWords[NAXIS1] )
        {
            m_file >> value;
            m_headerDATA[NAXIS1] = value;
            hsn++;
        }
        if( skwTemp == m_headerKeyWords[NAXIS2] )
        {

            m_file >> value;
            m_headerDATA[NAXIS2] = value;
            hsn++;
        }

        k++;
        m_file.clear();
        m_file.seekp( k * 80 );
    }
    if( FitsKeyWord_MAX != hsn + 1 ) return false;

    m_file.clear();
    m_file.seekp( (S32)m_file.tellp() + 80 );

    string voidString( 80, ' ' );
    string strTemp( 80, ' ' );
    while( voidString == strTemp )
        m_file.read( (C8*)strTemp.data(), 80 );

    m_file.clear();
    m_metadataSize = (S32)m_file.tellp() - 80;
    m_file.seekp( m_metadataSize );

    return true;
}

bool Fits::printfHeaderToFile()
{
    m_file.clear();
    m_file.seekg( 0 );

    S32 N1 = m_headerDATA[NAXIS1];
    S32 N2 = m_headerDATA[NAXIS2];
    string voidComment( 48, ' ' );
    string voidSting( 80, ' ' );
    m_metadataSize = 2880;

    for( S32 i = 0; i < FitsKeyWord_MAX - 1; i++ )
    {
        m_file << m_headerKeyWords[i];
        m_file.width( 21 );

        switch( i )
        {
            case SIMPLE:    m_file << "T";      break;
            case BITPIX:    m_file << 16;       break;
            case NAXIS:     m_file << 2;        break;
            case NAXIS1:    m_file << N1;       break;
            case NAXIS2:    m_file << N2;       break;
        }
        m_file << " /";
        m_file.write( voidComment.c_str(), 48 );
    }
    m_file.setf( ios::left ); m_file.width( 80 );
    m_file << m_headerKeyWords[END];
    for( S32 i = FitsKeyWord_MAX; i < m_metadataSize / 80; i++ )
        m_file.write( voidSting.c_str(), 80 );

    return true;
}

bool Fits::readDATAFromFile()
{
    S32 X = m_headerDATA[NAXIS1];
    S32 Y = m_headerDATA[NAXIS2];
    S32 cellSize = m_headerDATA[BITPIX] / 8;

    if( m_DATA )
        delete m_DATA;
    m_DATA = new MATRIX<F32>( Y, X );

    m_file.clear();
    m_file.seekp( m_metadataSize );

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            U16 temp;
            m_file.read( (char*)&temp, cellSize );
            PRC->swapBytes( &temp, 0, 1 );
            m_DATA[NULL][j][i] = (F32)temp;
        }

    return true;
}

bool Fits::writeDATAToFile()
{
    S32 X = m_headerDATA[NAXIS1];
    S32 Y = m_headerDATA[NAXIS2];
    S32 cellSize = 2;

    m_file.clear();
    m_file.seekg( m_metadataSize );

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            U16 temp = (U16)m_DATA[NULL][j][i];
            PRC->swapBytes( &temp, 0, 1 );
            m_file.write( (char*)&temp, cellSize );
        }
    return true;
}

void Fits::readFitsFile()
{
    m_file = fstream( m_fileName, ios_base::in | ios_base::binary );
    if( !m_file.is_open() )
        return;

    if( scanfHeaderFromFile() )
        readDATAFromFile();
    m_file.close();

}

void Fits::writeFitsFile()
{
    m_file = fstream( m_fileName, ios_base::out | ios_base::binary );
    if( !m_file.is_open() )
        return;

    if( printfHeaderToFile() )
        writeDATAToFile();
    m_file.close();
}
