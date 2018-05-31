#ifndef CATALOGGAIA2_H
#define CATALOGGAIA2_H

#include "Improvements.h"
#include "Astronomy/Fits.h"
#include "Astronomy/Catalog.h"
#include "Astronomy/CelestialBody.h"
#include "matrix.h"

class CatalogGAIA2 : public Catalog
{
public:
    virtual bool readFile( string fileName );
    bool readFile_RaDec( string fileName );
    matrix<F32> * CRTmatrixOfThis( S32 Y, S32 X );  //if PartGAIA!!!

    D64 minRa, maxRa;       //Feeeeee!.. I know!
    D64 minDec, maxDec;
};

#endif // CATALOGGAIA2_H
