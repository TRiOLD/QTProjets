#ifndef PROGRAM_H
#define PROGRAM_H

#include "Improvements.h"
#include "Astronomy/Fits.h"
#include "Astronomy/Catalog.h"
#include "Astronomy/CelestialBody.h"
#include "CatalogGAIA2.h"
#include "CatalogMyGC.h"
#include "matrix.h"
#include "MathMatrix.h"
#include "Algorithms.h"
#include "AlgMatrix.h"
#include "PictureObject.h"

////////////////////////////////////
class Program
{
public:
    Program();
    Program( S32 argc, C8* argv[] );
    ~Program();

private:
    STRS args;

public:
    void initialize();
    void shutdown();
    void process0();
    void process1();
};

#endif // PROGRAM_H
