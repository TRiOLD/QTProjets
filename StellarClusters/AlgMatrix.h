#ifndef ALGMATRIX_H
#define ALGMATRIX_H

#include "Improvements.h"
#include "Algorithms.h"
#include "matrix.h"
#include "MathMatrix.h"
#include "PictureObject.h"

class AlgMatrix : public Algorithms
{
public:
    static matrix<F32> * CRTgradient_OLSq2( matrix<F32> * data );
    static matrix<F32> * CRTdeviations_allX( matrix<F32> * data );
    static matrix<F32> * CRTsmooth_X( matrix<F32> * data, U16 coreSize );
    static matrix<F32> * CRTfilter_justFilter( matrix<F32> * data, U16 coreSize );
    static matrix<F32> * CRTconvolution_withGauss( matrix<F32> * data, U16 coreSize );

    static vector<PictureObject> searcherPictureObjects( matrix<F32> * data );
};

#endif // ALGMATRIX_H
