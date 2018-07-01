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

#include "Astronomy/CelestialBody.h"
#include "Algorithms.h"

////////////////////////////////////
class Catalog
{
public:
    Catalog()
    {
        m_header =
        "RA\tDEC\tmuRA\tmuDEC\teRA\teDEC\teMuRA\teMuDEC\tPLX\tMAG\tQUALITY\n";
    }
    virtual ~Catalog()
    {

    }

protected:
    std::string m_header;
    std::vector<CelestialBody> m_objects;

public:
    int getObjCount(){ return m_objects.size(); }
    CelestialBody * getObj( const int i ){ return &m_objects.at( i ); }

    void setupObject( CelestialBodyType type, std::string ID, const int n )
    {
        if( n >= 0 && n < getObjCount() )
        {
            getObj( n )->setHeader( &m_header );
            getObj( n )->setID( ID );
            getObj( n )->setType( type );
        }
    }

    void pushBack( CelestialBody body )
    {
        m_objects.push_back( body );
    }

    void pushBack( CatalogMyGC * cat )
    {
        S32 N = cat->getObjCount() - 1; //Костыль!!!
        S32 I0 = getObjCount();
        S32 I = I0 + N;
        m_objects.resize( I0 + N );
        for( S32 i = I0, j = 0; i < I; i++, j++ )
                m_objects[i] = *(cat->getObj( j ));
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
