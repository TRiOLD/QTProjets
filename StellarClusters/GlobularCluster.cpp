#include "GlobularCluster.h"

GlobularCluster::GlobularCluster() : CelestialBody()
{
    m_quality = 0.0;
    m_rad = 0.0;
    m_includedObjects = NULL;
    m_type = CelestialBodyType_Claster;
}

GlobularCluster::~GlobularCluster()
{

}

void GlobularCluster::init()
{

}

void GlobularCluster::init( U64 id, D64 ra, D64 dec, D64 prx, D64 rad, D64 muRa, D64 muDec, S32 quality )
{
    m_id = id;
    m_ra = ra;
    m_dec = dec;
    m_prx = prx;
    m_rad = rad;
    m_muRa = muRa;
    m_muDec = muDec;
    m_quality = quality;
}

string GlobularCluster::getAllParameters()
{
    string res;
    res = to_string( m_id ) + "\t" +
        to_string( m_ra ) + "\t" +
        to_string( m_dec ) + "\t" +
        to_string( m_prx ) + "\t" +
        to_string( m_rad ) + "\t" +
        to_string( m_muRa ) + "\t" +
        to_string( m_muDec ) + "\t" +
        to_string( m_quality );

    return res;
}
