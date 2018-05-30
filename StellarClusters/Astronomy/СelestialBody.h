////////////////////////////////////
//class CelestialBody
//
//C++98/03, C++11 (+STL)
//
//
//byTRiOLD -l-      04.18 upd: 05.18
////////////////////////////////////
#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

////////////////////////////////////
enum CelestialBodyParamMain
{
    RA, DEC, muRA, muDEC,
    eRA, eDEC, eMuRA, eMuDEC,
    PLX, ePLX, MAG, QUALITY,
    CelestialBodyParamMain_MAX
};
enum CelestialBodyType
{
    STAR, GALAXY, GCLASTER,
    UN_CelestialBodyType
};

////////////////////////////////////

class CelestialBody
{
public:
    CelestialBody()
    {
        _init();
    }
    CelestialBody( const int paramsCount )
    {
        _init();
        if( paramsCount > CelestialBodyParamMain_MAX )
            m_param.resize( paramsCount );
    }

private:
    std::vector<double> m_param;
    std::string * m_header;

    CelestialBodyType m_type;
    std::string m_ID;

private:
    void _init()
    {
        m_header = NULL;
        m_param.resize( CelestialBodyParamMain_MAX );
        m_type = UN_CelestialBodyType;
        m_ID = "UN0000000000";
    }

public:
    void setHeader( std::string * header ){ m_header = header; }
    std::string getHeader()
    {
        std::string header;
        if( m_header )
            header = *m_header;
        return header;
    }

    bool setAllParamMain( double ra, double dec, double muRa, double muDec,
                             double eRa, double eDec, double eMuRa, double eMuDec,
                                double mag, double quality)
    {
        m_param.at( RA ) = ra;          m_param.at( DEC ) = dec;
        m_param.at( muRA ) = muRa;      m_param.at( muDEC ) = muDec;
        m_param.at( eRA ) = eRa;        m_param.at( eDEC ) = eDec;
        m_param.at( eMuRA ) = eMuRa;    m_param.at( eMuDEC ) = eMuDec;
        m_param.at( MAG ) = mag;        m_param.at( QUALITY ) = quality;

        return true;
    }

    bool setParam( double value, const int i )
    {
        if( i < 0 || i > (int)(m_param.size()) )
            return false;
        m_param.at( i ) = value;
        return true;
    }
    double getParam( const int i )
    {
        if( i < 0 || i > (int)(m_param.size()) )
            return 0.0;
        return m_param[i];
    }

    void setType( CelestialBodyType type ){ m_type = type; }
    void setID( std::string ID ){ m_ID = ID; }
    CelestialBodyType getType(){ return m_type; }
    std::string getID(){ return m_ID; }
};
////////////////////////////////////
