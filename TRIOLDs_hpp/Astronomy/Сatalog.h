////////////////////////////////////
//class Catalog
//
//C++98/03, C++11 (+STL)
//incl: Astronomy/СelestialBody.h
//      Algorithms.h
//
//
//byTRiOLD -l-      04.18 upd: 05.18
////////////////////////////////////
#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>

#include "Astronomy/СelestialBody.h"
#include "Algorithms.h"

////////////////////////////////////
class Catalog
{
public:
    Catalog()
    {
        _init();
    }
    virtual ~Catalog()
    {

    }

protected:
    std::string m_header;
    std::vector<CelestialBody> m_objects;

private:
    std::vector<double> m_maxParams;
    std::vector<double> m_minParams;

protected:
    void _init()
    {
        m_header = "RA\tDEC\tmuRA\tmuDEC\teRA\teDEC\teMuRA\teMuDEC\tMAG\tQUALITY\n";
    }

public:
    int getObjCount(){ return m_objects.size(); }
    CelestialBody * getObj( const int i ){ return &m_objects.at( i ); }

    double getMinParam( const int i )
    {
        if( i < 0 || i > (int)(m_minParams.size()) )
            return 0.0;
        return m_minParams.at( i );
    }
    double getMaxParam( const int i )
    {
        if( i < 0 || i > (int)(m_maxParams.size()) )
            return 0.0;
        return m_maxParams.at( i );
    }

    void calculateMinMaxParamsMain()
    {
        if( !m_minParams.empty() && !m_maxParams.empty() )
            return;

        int I = CelestialBodyParamMain_MAX;
        int J = getObjCount();
        m_minParams.resize( I, 2147483647 );
        m_maxParams.resize( I, -2147483647 );

        for( int j = 0; j < J; j++ )
            for( int i = 0; i < I; I++ )
            {
                double val = getObj( j )->getParam( i );
                if( val < m_minParams.at( i ) )	m_minParams.at( i ) = val;
                if( val > m_maxParams.at( i ) )	m_maxParams.at( i ) = val;
            }
    }

    void setupObject( CelestialBodyType type, std::string ID, const int n )
    {
        if( n >= 0 && n < getObjCount() )
        {
            getObj( n )->setHeader( &m_header );
            getObj( n )->setID( ID );
            getObj( n )->setType( type );
        }
    }

    virtual bool readFile( std::string fileName )
    {
        std::fstream file;
        file.open( fileName, std::ios_base::in );
        if( !file.is_open() )   return false;

        unsigned int rows = 0;
        std::string fileString;
        while( std::getline( file, fileString ) )
            rows++;

        file.close();   m_objects.resize( rows );
        file.open( fileName, std::ios_base::in );

        std::getline( file, m_header );
        unsigned int colms = Algorithms::calculateWordsInSrting( m_header );

        std::string ID; unsigned int k = 0;
        while( std::getline( file, fileString ) )
        {
            strings W = Algorithms::divideByWordsSrting( fileString, colms );
            ID = "OBJ" + std::to_string( k+1 );
            setupObject( UN_CelestialBodyType, ID, k );
            for( int i = 1; i < CelestialBodyParamMain_MAX; i++ )
                m_objects[k].setParam( stod( W[i] ), i );

            k++;
        }

        file.close();
        return true;
    }

    virtual bool writeToFile( std::string fileName )
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
            for( int i = 0; i < CelestialBodyParamMain_MAX; i++ )
            {
                double value = m_objects[k].getParam( i );
                file << std::fixed << std::setprecision( 10 )
                     << value << "\t\t";
            }
            file << endl;
            k++;
        }

        return true;
    }
};
