#include "AlgMatrix.h"

matrix<F32> * AlgMatrix::CRTgradient_OLSq2( matrix<F32> * data )
{
    const S32 X = data->getSizeX(), Y = data->getSizeY();
    matrix<F32> * resData = new matrix<F32>( Y, X );

    const S32 N = 8;         // Порядок полинома
    const S32 M = X * Y;    // всего уравнений
    const S32 K = 10;      // размер пикселя K*K (для сред. значения)

    F32 lM = sqrt( (F32)M );
    F32 stepX = X / lM;
    F32 stepY = Y / lM;

    S32 sizeP = 0;  S32 m = 0;
    for( S32 n = 0; n <= N+1; n++ )
        sizeP += n;
    auto polynomial2D = [sizeP]( int y, int x )
    {
        MathMatrix P( 1, sizeP );
        S32 k = 0;
        for( S32 j = 0; j < N; j++ )
            for( S32 i = 0; i < j+1; i++ )
            {
                D64 valueX = pow( (D64)x, (D64)i );
                D64 valueY = pow( (D64)y, (D64)j );
                P[0][k] = (F32)valueX * (F32)valueY;
                k++;
            }

        return P;
    };

    MathMatrix XY( M, N );
    MathMatrix Z( M, 1 );

    m = 0;
    for( F32 j = 0; j < Y; j+= stepY )
        for( F32 i = 0; i < X; i+= stepX )
        {
            S32 k = 0;  D64 dataPix = 0.0;
            for( S32 q = j - K/2; q < j + K/2; q++ )
                for( S32 p = i - K/2; p < i + K/2; p++ )
                    if( p > 0 && p < X && q > 0 && q < Y )
                        if( (S32)(*data)[q][p] )
                        {
                            dataPix += ((D64)(*data)[q][p] - dataPix) / ((D64)k + 1.0);
                            k++;
                        }
            Z[m][0] = dataPix;      // Заполнение N-елементов значениями f(x,y)


            MathMatrix P = polynomial2D( j+1, i+1 );
            for( S32 n = 0; n < N; n++ )
                XY[m][n] = P[0][n]; // Заполнение N-елементов матрицы по X

            m++;
        }

    MathMatrix XYtr = XY;	XYtr.toTranspose(); // МНК
    MathMatrix XYtrXYinv = XYtr * XY;	XYtrXYinv.toInvers();
    MathMatrix A = XYtrXYinv * XYtr * Z;

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            MathMatrix P = polynomial2D( j+1, i+1 );
            for( S32 n = 0; n < N; n++ )
                (*resData)[j][i] += A[n][0] * P[0][n];
        }

    return resData;
}


matrix<F32> * AlgMatrix::CRTdeviations_allX( matrix<F32> * data )
{
    const S32 X = data->getSizeX(), Y = data->getSizeY();
    matrix<F32> * resData = new matrix<F32>( Y, X );

    vector<F32> Xavers( X ), Yavers( Y ), Xvarians( X ), Yvarians( Y );

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
            Yavers[j] += ((D64)(*data)[j][i] - Yavers[j]) / ((D64)i + 1.0);
    for( S32 i = 0; i < X; i++ )
        for( S32 j = 0; j < Y; j++ )
            Xavers[i] += ((D64)(*data)[j][i] - Xavers[i]) / ((D64)j + 1.0);
    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
            Yvarians[j] += (pow( (D64)(*data)[j][i] - Yavers[j], 2 )) / Y;
    for( S32 i = 0; i < X; i++ )
        for( S32 j = 0; j < Y; j++ )
            Xvarians[i] += (pow( (D64)(*data)[j][i] - Xavers[i], 2 )) / X;

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            D64 sigma = sqrt( (Xvarians[i] + Yvarians[j]) / 2.0 );
            if( (*data)[j][i] < 3 * sigma )
                (*resData)[j][i] = 0.0f;
            else
                (*resData)[j][i] = (*data)[j][i];
        }

    return resData;
}


matrix<F32> * AlgMatrix::CRTsmooth( matrix<F32> * data, U16 coreSize )
{
    const S32 X = data->getSizeX(), Y = data->getSizeY();
    matrix<F32> * resData = new matrix<F32>( Y, X );

    const U16 cSizeL = coreSize * coreSize;
    const S32 halfL = S32(coreSize / 2.0 + 0.5);

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            vector<F32> core( cSizeL );

            U16 k = 0;
            for( S32 m = 0, q = -halfL; m < coreSize; m++, q++ )
                for( S32 n = 0, p = -halfL; n < coreSize; n++, p++ )
                    if( i+p >= 0 && i+p < X && j+q >= 0 && j+q < Y )
                    {
                        core[k] = (*data)[j+q][i+p];
                        k++;
                    }

            (*resData)[j][i] = Algorithms::average( core, k );
        }

    return resData;
}


matrix<F32> * AlgMatrix::CRTsmooth_X( matrix<F32> * data, U16 coreSize )
{
    const S32 X = data->getSizeX(), Y = data->getSizeY();
    matrix<F32> * resData = new matrix<F32>( Y, X );

    const int halfL = int(coreSize / 2.0 + 0.5);

    for( int j = 0; j < Y; j++ )
        for( int i = 0; i < X; i++ )
        {
            vector<float> core( 5 * coreSize );
            int k = 0;

            for( int m = 0, q = -halfL; m < coreSize; m++, q++ )
                if( j+q >= 0 && j+q < Y )
                    if( (*data)[j+q][i] > 0.0f )
                    {
                        core[k] = (*data)[j+q][i];
                        k++;
                    }
            for( int n = 0, p = -halfL; n < coreSize; n++, p++ )
                if( i+p >= 0 && i+p < X )
                    if( (*data)[j][i+p] > 0.0f )
                    {
                        core[k] = (*data)[j][i+p];
                        k++;
                    }
            for( int nm = 0, pq = -halfL; nm < coreSize; nm++, pq++ )
                if( i+pq >= 0 && i+pq < X && j+pq >= 0 && j+pq < Y )
                    if( (*data)[j+pq][i+pq] > 0.0f )
                    {
                        core[k] = (*data)[j+pq][i+pq];
                        k++;
                    }
            for( int nm = 0, pq = -halfL; nm < coreSize; nm++, pq++ )
                if( i+halfL-pq >= 0 && i+halfL-pq < X && j+pq >= 0 && j+pq < Y )
                    if( (*data)[j+pq][i+halfL-pq] > 0.0f )
                    {
                        core[k] = (*data)[j+pq][i+halfL-pq];
                        k++;
                    }

            (*resData)[j][i] = average( core, k );
        }

    return resData;
}


matrix<F32> * AlgMatrix::CRTfilter_justFilter( matrix<F32> * data, U16 coreSize )
{
    const S32 X = data->getSizeX(), Y = data->getSizeY();
    matrix<F32> * resData = new matrix<F32>( Y, X );

    const U16 cSizeL = coreSize * coreSize;
    const S32 halfL = S32(coreSize / 2.0 + 0.5);

    const S32 N = X * Y; S32 n = 0;
    vector<F32> mainCore( N );
    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            mainCore[n] = (*data)[j][i];
            n++;
        }
    D64 mainSerSum = average( mainCore, N );

    const D64 dev = 0.6;     //Чувствительность

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            vector<F32> core( cSizeL );

            U16 k = 0;
            for( S32 m = 0, q = -halfL; m < coreSize; m++, q++ )
                for( S32 n = 0, p = -halfL; n < coreSize; n++, p++ )
                    if( i+p >= 0 && i+p < X && j+q >= 0 && j+q < Y )
                    {
                        core[k] = (*data)[j+q][i+p];
                        k++;
                    }
            sort( core.begin(), core.begin() + k );

            D64 aver = 0.0;
            int r = 0;
            bool check = false;
            while( !check )
            {
                aver = average( core, k );
                if( aver > mainSerSum + mainSerSum * dev ||
                    aver < mainSerSum - mainSerSum * dev )
                {
                     core[r] = mainSerSum;
                     r++;
                     core[k-r] = mainSerSum;
                }
                else check = true;
            }

            (*resData)[j][i] = (F32)aver;
        }

    return resData;
}


matrix<F32> * AlgMatrix::CRTconvolution_withGauss( matrix<F32> * data, U16 coreSize )
{
    const S32 X = data->getSizeX(), Y = data->getSizeY();
    matrix<F32> * resData = new matrix<F32>( Y, X );

    const U16 cSizeX = coreSize;
    const U16 cSizeY = coreSize;
    const D64 SigX = 1.0;//cSizeX / 5.0;
    const D64 SigY = 1.0;//cSizeY / 5.0;

    matrix<D64> core( cSizeY, cSizeX );
    const S32 halfX = S32(cSizeX / 2.0 + 0.5);
    const S32 halfY = S32(cSizeY / 2.0 + 0.5);

    D64 sumcore = 0;
    for( U16 m = 0; m < cSizeY; m++ )
        for( U16 n = 0; n < cSizeX; n++ )
        {
            D64 constitX = exp( pow( (D64)n - halfX + 0.5, 2.0 ) / 2 / pow( SigX, 2.0 ) );
            D64 constitY = exp( pow( (D64)m - halfY + 0.5, 2.0 ) / 2 / pow( SigY, 2.0 ) );
            core[m][n] = 1.0 / constitX / constitY / (2 * PI * SigX * SigY);
            sumcore += core[m][n];
        }

    for( U32 m = 0; m < cSizeY; m++ )
        for( U32 n = 0; n < cSizeX; n++ )
            core[m][n] /= sumcore;     //ДоНормировка

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            S32 a = i - halfX; if( a <= 0 )  a = 0;
            S32 b = i + halfX; if( b >= X )  b = X - 1;
            S32 c = j - halfY; if( c <= 0 )  c = 0;
            S32 d = j + halfY; if( d >= Y )  d = Y - 1;
            for( S32 m = 0, q = -halfY; m < cSizeY; m++, q++ )
                for( S32 n = 0, p = -halfX; n < cSizeX; n++, p++ )
                    if( i+p >= a && i+p <= b && j+q >= c && j+q <= d )
                        (*resData)[j][i] += (*data)[j+q][i+p] * (F32)core[m][n];
        }

    return resData;
}


matrix<F32> * AlgMatrix::CRTnormalization_pow2Max( matrix<F32> * data )
{
    const S32 X = data->getSizeX(), Y = data->getSizeY();
    matrix<F32> * resData = new matrix<F32>( Y, X );
    matrix<F32> * tempData = new matrix<F32>( Y, X );

    F32 minMainVal = 2147483648, maxMainVal = 0;
    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            if( (*data)[j][i] > maxMainVal )    maxMainVal = (*data)[j][i];
            if( (*data)[j][i] < minMainVal )    minMainVal = (*data)[j][i];
        }
    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
            (*tempData)[j][i] = (*data)[j][i] - minMainVal;
    maxMainVal -= minMainVal;

    F32 maxLoc = 0;
    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            (*tempData)[j][i] *= (*tempData)[j][i];
            if( (*tempData)[j][i] > maxLoc )    maxLoc = (*tempData)[j][i];
        }
    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
            (*resData)[j][i] = (*tempData)[j][i] / maxLoc * maxMainVal;

    delete tempData;
    return resData;
}


matrix<F32> * AlgMatrix::CRTnormalization_justNorm( matrix<F32> * data )
{
    const S32 X = data->getSizeX(), Y = data->getSizeY();
    matrix<F32> * resData = new matrix<F32>( Y, X );
    matrix<F32> * tempData = new matrix<F32>( *data );
    const S32 N = X * Y;

    const D64 epsilon = 0.0001; // придел сходимость дисперсии

    D64 averCore = 0.0, sigmaCore = 65536, sigmaTempCore = 0.0;
    while( abs( sigmaCore - sigmaTempCore ) > epsilon )
    {
        vector<F32> core( N );
        S32 k = 0;
        for( S32 j = 0; j < Y; j++ )
            for( S32 i = 0; i < X; i++ )
                if( (*tempData)[j][i] >= 1.0f )
                {
                    core[k] = (*tempData)[j][i];
                    k++;
                }

        averCore = Algorithms::average( core, k );
        sigmaTempCore = sigmaCore;
        sigmaCore = Algorithms::stdDeviation( core, averCore, k );

        for( S32 j = 0; j < Y; j++ )
            for( S32 i = 0; i < X; i++ )
                if( (*tempData)[j][i] > averCore + 3 * sigmaCore )
                    (*tempData)[j][i] = averCore;

        cout << endl << averCore << " " << sigmaCore << endl;
    }

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            (*resData)[j][i] = (*data)[j][i] - (averCore + 5 * sigmaCore);
            if( (*resData)[j][i] < 0.0f )   (*resData)[j][i] = 0.0f;
        }

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
            if( (*resData)[j][i] >= 1.0 )
            {
                S32 iter = 0;
                S32 x1 = i - 1, x2 = i + 1;
                S32 y1 = j - 1, y2 = j + 1;
                if( x1 >= 0 )
                    if( (*resData)[j][x1] > 1 ) iter++;
                if( x2 < X )
                    if( (*resData)[j][x2] > 1 ) iter++;
                if( y1 >= 0 )
                    if( (*resData)[y1][i] > 1 ) iter++;
                if( y2 < Y )
                    if( (*resData)[y2][i] > 1 ) iter++;

                if( !iter )
                    (*resData)[j][i] = 0.0f;
            }

    delete tempData;
    return resData;
}


vector<PictureObject> AlgMatrix::searcherPictureObjects( matrix<F32> * data )
{
    const S32 X = data->getSizeX(), Y = data->getSizeY();
    matrix<F32> * tempData = new matrix<F32>( *data );
    vector<PictureObject> res;

    function<PictureObject( S32, S32 )> chekSpot;
    chekSpot = [&chekSpot, tempData, X, Y]( S32 y, S32 x )
    {
        PictureObject spot;
        spot.push( PictureObject::Dot( x, y, (*tempData)[y][x] ) );
        (*tempData)[y][x] = 0.0f;
        S32 x1 = x - 1, x2 = x + 1;
        S32 y1 = y - 1, y2 = y + 1;
        if( x1 >= 0 )
            if( S32((*tempData)[y][x1]) > 0 ) spot.push( chekSpot( y, x1 ) );
        if( x2 < X )
            if( S32((*tempData)[y][x2]) > 0 ) spot.push( chekSpot( y, x2 ) );
        if( y1 >= 0 )
            if( S32((*tempData)[y1][x]) > 0 ) spot.push( chekSpot( y1, x ) );
        if( y2 < Y )
            if( S32((*tempData)[y2][x]) > 0 ) spot.push( chekSpot( y2, x ) );

        return spot;
    };

    res.resize( 1000 ); S32 k = 0;
    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
            if( S32((*tempData)[j][i]) > 0 )
            {
                res[k] = chekSpot( j, i );
                k++;
                if( k == 1000 ) return res;
            }

    res.resize( k );
    return res;
}
