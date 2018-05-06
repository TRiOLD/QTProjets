////////////////////////////////////
//class Catalog
//
//C++98/03, C++11 (+STL)
//incl: Astronomy/СelestialBody.h
//
//
//byTRiOLD -l-      04.18 upd: 05.18
////////////////////////////////////
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "Astronomy/СelestialBody.h"

////////////////////////////////////
class Catalog
{
public:
    Catalog()
    {
        _init();
    }

protected:
    std::string m_header;
    std::vector<CelestialBody> m_objects;

private:
    std::vector<double> m_maxParams;
    std::vector<double> m_minParams;

private:
    void _init()
    {

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
        m_minParams.resize( I, 268435456.0 );
        m_maxParams.resize( I, -268435456.0 );

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
        if( n > 0 && n < getObjCount() )
        {
            getObj( n )->setHeader( &m_header );
            getObj( n )->setID( ID );
            getObj( n )->setType( type );
        }
    }

    virtual bool readFile( std::string fileName )
    {


        return true;
    }

    virtual bool writeToFile( std::string fileName )
    {


        return true;
    }
};
