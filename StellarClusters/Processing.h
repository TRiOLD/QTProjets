////////////////////////////////////
#pragma once

#include "Improvements.h"
#include "CelestialBody.h"
#include "GlobularCluster.h"
#include "Catalog.h"
#include "Fits.h"

////////////////////////////////////
enum CelestialBodyType;
class CelestialBody;
class GlobularCluster;
class Catalog;
class Fits;

////////////////////////////////////
struct Dot
{
    U16 x;
    U16 y;
    U16 size;
};

struct Spot
{
    U16 x;
    U16 y;
    U16 sizeX;
    U16 sizeY;
    U16 sizeZ;
};

////////////////////////////////////
class Processing
{
public:
    Processing();
    ~Processing();

public:
    string FILENAME;
    string PATHTOFILE;

public:
    void swapBytes( void * buf, S32 num_b1, S32 num_b2 );
    bool compare( D64 a, D64 b, D64 eps );
    bool compareSrtings( string &str1, string &str2, U32 length );
    STRS divideByWordsSrtings( string &str, U32 wrdsMax );
    D64 average( vector<F32> &arr, U32 n );
    D64 median( vector<F32> &arr, U32 n );
    D64 stdDeviation( vector<F32> &arr, D64 arrMean, U32 n );

    MATRIX<F32> * transpose( MATRIX<F32> * data );

private: // какашкодские методы
    vector<Dot> recursChecking( MATRIX<U16> * data, Dot dot, U16 minValue, vector<Dot> adjacentDots );
    vector<Dot> checkingAdjacentCells( MATRIX<U16> * data, Dot dot, U16 minValue );

public:
    MATRIX<F32> * convolution_Gauss( MATRIX<F32> * data, U16 coreSize );
    MATRIX<F32> * filter_Median( MATRIX<F32> * data, U16 coreSize );
    MATRIX<F32> * filter_Average( MATRIX<F32> * data, U16 coreSize );
    MATRIX<F32> * filter_justFilter( MATRIX<F32> * data, U16 coreSize );
    MATRIX<F32> * filter_InclinedFilter( MATRIX<F32> * data, U16 coreSize );

    MATRIX<F32> * normalization( MATRIX<F32> * data, F32 norm );

    MATRIX<F32> * interpolation_Newton1( MATRIX<F32> * data );
    MATRIX<F32> * interpolation_Newton2( MATRIX<F32> * data );
    MATRIX<F32> * interpolation_Newton3( MATRIX<F32> * data );
    MATRIX<F32> * aproximation_OLSq2( MATRIX<F32> * data );

    vector<Dot> clasteroider( MATRIX<F32> * data ); // какашкодский метод

    vector<Spot> searcher( MATRIX<F32> * data );

    void intrFunction( Catalog * GCcat, string fileNameCat );

    Catalog * dedectedGlobularClasters( Catalog * inCat );
    Catalog * SearchingStarsInGlobularClaster( Catalog * inCat, GlobularCluster * claster );

    Fits * catalogueToFits( Catalog * inCat );

public:
/*
    Catalog * createGlobularClaster( U32 r, U32 n, D64 quality );
    Catalog * dedectedGlobularClasters_method1( Catalog * inCat );
    Catalog * dedectedGlobularClasters_method2( Catalog * inCat );
    Catalog * dedectedGlobularClasters_method3( Catalog * inCat );
    Catalog * dedectedGlobularClasters_method4( Catalog * inCat );

     void dedectedGlobularClasters_method5();
*/

    fstream m_file;
};
