#ifndef CLUSTERING_H
#define CLUSTERING_H

#include "Improvements.h"
#include "Algorithms.h"
#include "Astronomy/Catalog.h"
#include "Astronomy/CelestialBody.h"
#include "CatalogGAIA2.h"
#include "CatalogMyGC.h"

class Clustering : public Algorithms
{
public:
    static Catalog * CRTcatalog_TrueGC( Catalog * catPart, CelestialBody * gc );
};

#endif // CLUSTERING_H
