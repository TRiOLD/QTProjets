#ifndef CATALOGMYGC_H
#define CATALOGMYGC_H

#include "Improvements.h"
#include "Astronomy/Catalog.h"
#include "Astronomy/CelestialBody.h"
#include "Algorithms.h"
#include "PictureObject.h"


class CatalogMyGC : public Catalog
{
public:
    CatalogMyGC();

private:
    S32 m_params;

public:
    void addObjects( vector<PictureObject> spots, S32 numPart,  //Временно!
                     S32 minX, S32 maxX, S32 minY, S32 maxY,
                     D64 minA, D64 maxA, D64 minB, D64 maxB );

    void pushBack( CatalogMyGC * cat );

    virtual bool readFile( string fileName );
    virtual bool writeToFile( string fileName );
};

#endif // CATALOGMYGC_H
