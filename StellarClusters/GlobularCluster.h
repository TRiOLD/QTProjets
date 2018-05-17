////////////////////////////////////
#pragma once

#include "Improvements.h"
#include "CelestialBody.h"
#include "Catalog.h"

////////////////////////////////////
enum CelestialBodyType;
class CelestialBody;
class Catalog;

////////////////////////////////////
class GlobularCluster :
	public CelestialBody
{
public:
    GlobularCluster();
    ~GlobularCluster();
    virtual void init();
    void init( U64 id, D64 ra, D64 dec, D64 prx, D64 rad, D64 muRa, D64 muDec, S32 quality );
private:
	Catalog * m_includedObjects;
    D64 m_rad;
	D64 m_quality;

public:
    void setObj( Catalog * includedObjects ){ m_includedObjects = includedObjects; }
    Catalog * getObj(){ return m_includedObjects; }
    virtual string getAllParameters();
};

////////////////////////////////////
