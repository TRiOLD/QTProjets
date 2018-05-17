////////////////////////////////////
#pragma once

#include "Improvements.h"
#include "Catalog.h"
#include "Processing.h"
#include "CelestialBody.h"
#include "GlobularCluster.h"
#include "Fits.h"

////////////////////////////////////
class Catalog;
class Processing;
class CelestialBody;
class GlobularClusters;
class Fits;

////////////////////////////////////
class Program
{
public:
	Program();
	Program( S32 argc, C8* argv[] );
	~Program();

	void initialize();
	void shutdown();
	void process();

private:
	STRS args; 

private:
	Processing PRC;
};

////////////////////////////////////
