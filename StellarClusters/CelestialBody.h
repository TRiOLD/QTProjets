////////////////////////////////////
#pragma once

#include "Improvements.h"

////////////////////////////////////
enum CelestialBodyType
{
	CelestialBodyType_Star,
	CelestialBodyType_Galaxy,
	CelestialBodyType_Claster,

	CelestialBodyType_Unknown
};

class CelestialBody
{
public:
	CelestialBody();
	virtual ~CelestialBody();
	virtual void init();

    void init( 	U64 id, D64 ra, D64 dec, D64 prx,
				D64 muRa, D64 muDec,
                D64 mag,
                CelestialBodyType type );

protected:
	CelestialBodyType m_type;
    U64 m_id;
	D64 m_ra;
	D64 m_dec;
    D64 m_prx;
	D64 m_muRa;
	D64 m_muDec;
	D64 m_mag;

public:
    U64 getID(){ return m_id; }
	D64 getRa(){ return m_ra; }
	D64 getDec(){ return m_dec; }
    D64 getPrx(){ return m_prx; }
	D64 getMuRa(){ return m_muRa; }
	D64 getMuDec(){ return m_muDec; }
	D64 getMag(){ return m_mag; }
    CelestialBodyType getType(){ return m_type; }

    virtual string getAllParameters();
};

////////////////////////////////////
