////////////////////////////////////
#pragma once

#include "Improvements.h"
#include "Processing.h"
#include "CelestialBody.h"

////////////////////////////////////
class Processing;
class CelestialBody;
enum CelestialBodyType;

////////////////////////////////////
class Catalog
{
public:
	Catalog();
	~Catalog();
	void init();

private:
	fstream m_file;
	vector<CelestialBody> m_objects;

    D64 m_minRa;
    D64 m_maxRa;
    D64 m_minDec;
    D64 m_maxDec;
    D64 m_minMag;
    D64 m_maxMag;

public:
	void calculateMinMaxParameters();

public:
    CelestialBody getObject( S32 n ){ return m_objects.at( n ); }
	U32 getObjectCount(){ return m_objects.size(); }
    D64 getMinRa(){ return m_minRa; }
    D64 getMaxRa(){ return m_maxRa; }
    D64 getMinDec(){ return m_minDec; }
    D64 getMaxDec(){ return m_maxDec; }
    D64 getMinMag(){ return m_minMag; }
    D64 getMaxMag(){ return m_maxMag; }

    void addObjects( Catalog * object );
    void addObject( CelestialBody object ){ m_objects.emplace_back( object ); }
	void replaceObject( CelestialBody object, S32 n ){ m_objects.at( n ) = object; }
	void deleteObject( S32 n ){ m_objects.erase( m_objects.begin() + n ); }

	bool writeToFile( string fileName );
	bool readFile( string fileName );

    bool readFile_pmaPart( string fileName );
    bool readFile_gaiaPart( string fileName );
    Catalog * getSubCatalog( D64 minRa, D64 maxRa, D64 minDec, D64 maxDec );
};

////////////////////////////////////
