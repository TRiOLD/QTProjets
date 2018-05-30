////////////////////////////////////
//class Fits
//
//C++98/03, C++11 (+STL)
//incl: matrix.h
//      Algorithms.h
//
//
//byTRiOLD -l-      01.18 upd: 05.18
////////////////////////////////////
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "matrix.h"
#include "Algorithms.h"

////////////////////////////////////
enum FitsKeyWord
{
    SIMPLE, BITPIX,
    NAXIS, NAXIS1, NAXIS2,
    END,
    FitsKeyWord_MAX
};

////////////////////////////////////
class Fits
{
public:
    Fits()
    {
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
    }
    ~Fits()
    {

    }

private:
    std::fstream m_file;
    std::string m_fileName;
    int m_metadataSize;

    std::string m_headerKeyWords[FitsKeyWord_MAX];
    int m_headerDATA[FitsKeyWord_MAX];
    matrix<float> * m_DATA;

private:
    bool _scanfHeaderFromFile()
    {
        m_file.clear();
        m_file.seekp( 0 );

        std::string skwTemp( 9, ' ' );
        int hsn = 0, k = 0;

        while( m_headerKeyWords[END] != skwTemp )
        {
            if( m_file.eof() )	return false;
            m_file.read( (char*)skwTemp.data(), 9 );

            int value;
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
        m_file.seekp( (int)m_file.tellp() + 80 );

        std::string voidString( 80, ' ' );
        std::string strTemp( 80, ' ' );
        while( voidString == strTemp )
            m_file.read( (char*)strTemp.data(), 80 );

        m_file.clear();
        m_metadataSize = (int)m_file.tellp() - 80;
        m_file.seekp( m_metadataSize );

        return true;
    }

    bool _printfHeaderToFile()
    {
        m_file.clear();
        m_file.seekg( 0 );

        int N1 = m_headerDATA[NAXIS1];
        int N2 = m_headerDATA[NAXIS2];
        std::string voidComment( 48, ' ' );
        std::string voidSting( 80, ' ' );
        m_metadataSize = 2880;

        for( int i = 0; i < FitsKeyWord_MAX - 1; i++ )
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
        m_file.setf( std::ios::left ); m_file.width( 80 );
        m_file << m_headerKeyWords[END];
        for( int i = FitsKeyWord_MAX; i < m_metadataSize / 80; i++ )
            m_file.write( voidSting.c_str(), 80 );

        return true;
    }

    bool _readDATAFromFile()
    {
        int X = m_headerDATA[NAXIS1];
        int Y = m_headerDATA[NAXIS2];
        int cellSize = m_headerDATA[BITPIX] / 8;

        m_DATA->create( Y, X, false );

        m_file.clear();
        m_file.seekp( m_metadataSize );

        for( int j = 0; j < Y; j++ )
            for( int i = 0; i < X; i++ )
            {
                unsigned short int temp;
                m_file.read( (char*)&temp, cellSize );
                Algorithms::swapBytes( &temp, 0, 1 );
                (*m_DATA)[j][i] = (float)temp;
            }

        return true;
    }

    bool _writeDATAToFile()
    {
        int X = m_headerDATA[NAXIS1];
        int Y = m_headerDATA[NAXIS2];
        int cellSize = 2;

        m_file.clear();
        m_file.seekg( m_metadataSize );

        for( int j = 0; j < Y; j++ )
            for( int i = 0; i < X; i++ )
            {
                unsigned short int temp = (unsigned short int)((*m_DATA)[j][i]);
                Algorithms::swapBytes( &temp, 0, 1 );
                m_file.write( (char*)&temp, cellSize );
            }
        return true;
    }

public:
    bool readFitsFile()
    {
        m_file = std::fstream( m_fileName, std::ios_base::in | std::ios_base::binary );
        if( !m_file.is_open() )
            return false;

        if( _scanfHeaderFromFile() )
            _readDATAFromFile();
        else
            return false;

        m_file.close();
        return true;
    }

    bool writeFitsFile()
    {
        m_file = std::fstream( m_fileName, std::ios_base::out | std::ios_base::binary );
        if( !m_file.is_open() )
            return false;

        if( _printfHeaderToFile() )
            _writeDATAToFile();
        else
            return false;

        m_file.close();
        return true;
    }

    void setFile( std::string fileName ){ m_fileName = fileName; }
    matrix<float> * getPtrDATA(){ return m_DATA; }
    void setPtrDATA( matrix<float> * DATA )
    {
        m_DATA = DATA;
        m_headerDATA[NAXIS1] = DATA->getSizeX();
        m_headerDATA[NAXIS2] = DATA->getSizeY();
    }
};
