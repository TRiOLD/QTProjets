#include "CelestialBody.h"

CelestialBody::CelestialBody()
{
    m_id = 0;
    m_ra = 0.0;
    m_dec = 0.0;
    m_muRa = 0.0;
    m_muDec = 0.0;
    m_mag = 0.0;
    m_type = CelestialBodyType_Unknown;
}

CelestialBody::~CelestialBody()
{

}

void CelestialBody::init()
{

}

void CelestialBody::init( U64 id, D64 ra, D64 dec, D64 prx, D64 muRa, D64 muDec, D64 mag, CelestialBodyType type )
{
    m_id = id;
    m_ra = ra;
    m_dec = dec;
    m_prx = prx;
    m_muRa = muRa;
    m_muDec = muDec;
    m_mag = mag;
    m_type = type;
}

string CelestialBody::getAllParameters()
{
    string res;
    res = to_string( m_id ) + "\t" +
        to_string( m_ra ) + "\t" +
        to_string( m_dec ) + "\t" +
        to_string( m_prx ) + "\t" +
        to_string( m_muRa ) + "\t" +
        to_string( m_muDec ) + "\t" +
        to_string( m_mag );

    return res;
}
