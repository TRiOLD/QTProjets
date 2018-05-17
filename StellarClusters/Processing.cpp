#include "Processing.h"

#include "IN_conv_byVlad.h"
#include "IN_mnk_byVova.h"

Processing::Processing()
{

}

Processing::~Processing()
{

}

////////////////////////////////////
////////////////////////////////////
void Processing::swapBytes( void * buf, int num_b1, int num_b2 )
{
    char *p = (char*)buf;
    char temp = p[num_b1];
    p[num_b1] = p[num_b2];
    p[num_b2] = temp;
    buf = p;
}

bool Processing::compare( D64 a, D64 b, D64 eps )
{
    if( abs( a - b ) < eps )	return true;
    if( abs( a - b ) > eps )	return false;
    return false;
}

bool Processing::compareSrtings( string &str1, string &str2, U32 length )
{
    if( str1.length() < length || str2.length() < length )
        return false;

    for( U32 i = 0; i < length; i++ )
        if( str1[i] != str2[i] )
            return false;

    return true;
}

STRS Processing::divideByWordsSrtings( string &str, U32 wrdsMax )
{
    STRS res( wrdsMax );
    string word; U32 s = 0, w = 0, k = 0;
    for( U32 i = 0; i < str.size(); i++ )
        if( str[i] == ' ' || str[i] == '\n' || str[i] == '\t' )
        {
            if( w )
            {
                word = str.substr( s, w );
                res[k] = word;
                k++;
                if( k == wrdsMax )  break;
            }
            w = 0;
        }
        else
        {
            if( !w )    s = i;
            w++;
        }
    return res;
}

D64 Processing::average( vector<F32> &arr, U32 n )
{
    D64 res = 0;
    for( U32 i = 0; i < n; i++ )
        res += ((D64)arr[i] - res) / ((D64)i + 1.0);
    return res;
}

D64 Processing::median( vector<F32> &arr, U32 n )
{
    sort( arr.begin(), arr.begin() + n );
    D64 res;
    if( n % 2 )
    res = (arr[n / 2] + arr[n / 2 + 1]) / 2.0;
    else res = arr[n / 2];
    return res;
}

D64 Processing::stdDeviation( vector<F32> &arr, D64 arrMean, U32 n )
{
    D64 res = 0;
    for( U32 i = 0; i < n; i++ )
        res += (pow( (D64)arr[i] - arrMean, 2 )) / n;
    return sqrt( res );
}

MATRIX<F32> * Processing::transpose( MATRIX<F32> * data )
{
    const U16 Y = data->m_x, X = data->m_y;
    MATRIX<F32> * resDATA = new MATRIX<F32>( Y, X );

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
            resDATA[NULL][j][i] = data[NULL][i][j];

    return resDATA;
}

////////////////////////////////////
////////////////////////////////////
MATRIX<F32> * Processing::convolution_Gauss( MATRIX<F32> * data, U16 coreSize )
{
    const U16 cSizeX = coreSize;
    const U16 cSizeY = coreSize;
    const D64 SigX = cSizeX / 5.0;
    const D64 SigY = cSizeY / 5.0;

    MATRIX<D64> core( cSizeY, cSizeX );
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

    const U16 X = data->m_x, Y = data->m_y;
    MATRIX<F32> * resDATA = new MATRIX<F32>( Y, X );

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            resDATA[NULL][j][i] = 0.0;
            S32 a = i - halfX; if( a <= 0 )  a = 0;
            S32 b = i + halfX; if( b >= X )  b = X - 1;
            S32 c = j - halfY; if( c <= 0 )  c = 0;
            S32 d = j + halfY; if( d >= Y )  d = Y - 1;
            for( S32 m = 0, q = -halfY; m < cSizeY; m++, q++ )
                for( S32 n = 0, p = -halfX; n < cSizeX; n++, p++ )
                    if( i+p >= a && i+p <= b && j+q >= c && j+q <= d )
                        resDATA[NULL][j][i] += data[NULL][j+q][i+p] * (F32)core[m][n];
        }

    return resDATA;
}

////////////////////////////////////
MATRIX<F32> * Processing::filter_Median( MATRIX<F32> * data, U16 coreSize )
{
    const U16 X = data->m_x, Y = data->m_y;
    MATRIX<F32> * resDATA = new MATRIX<F32>( Y, X );

    const U16 cSizeL = coreSize * coreSize;
    const S32 halfL = S32(coreSize / 2.0 + 0.5);

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            vector<F32> core( cSizeL );

            D64 med = 0.0;
            D64 sigma = 10000.0;
            D64 sigmaTemp = 0.0;
            while( abs( sigma - sigmaTemp ) > 0.0001 )
            {
             U16 k = 0;
             for( S32 m = 0, q = -halfL; m < coreSize; m++, q++ )
                 for( S32 n = 0, p = -halfL; n < coreSize; n++, p++ )
                     if( i+p >= 0 && i+p < X && j+q >= 0 && j+q < Y )
                     {
                         core[k] = data[NULL][j+q][i+p];
                         k++;
                     }

             med = median( core, k );
             sigmaTemp = sigma;
             sigma = stdDeviation( core, med, k );
             for( int i = 0; i < k; i++ )
                if( core[i] + 1 * sigma > med
                 || core[i] - 1 * sigma < med )
                    core[i] = (F32)med;
            }

            resDATA[NULL][j][i] = (F32)med;
        }

     return resDATA;
}

////////////////////////////////////
MATRIX<F32> * Processing::filter_Average( MATRIX<F32> * data, U16 coreSize )
{
    const U16 X = data->m_x, Y = data->m_y;
    MATRIX<F32> * resDATA = new MATRIX<F32>( Y, X );

    const U16 cSizeL = coreSize * coreSize;
    const S32 halfL = S32(coreSize / 2.0 + 0.5);

     for( S32 j = 0; j < Y; j++ )
         for( S32 i = 0; i < X; i++ )
         {
             vector<F32> core( cSizeL );

             D64 serSum = 0.0;
             D64 sigma = 10000.0;
             D64 sigmaTemp = 0.0;
             while( abs( sigma - sigmaTemp ) > 0.0001 )
             {
                 U16 k = 0;
                 for( S32 m = 0, q = -halfL; m < coreSize; m++, q++ )
                     for( S32 n = 0, p = -halfL; n < coreSize; n++, p++ )
                         if( i+p >= 0 && i+p < X && j+q >= 0 && j+q < Y )
                         {
                             core[k] = data[NULL][j+q][i+p];
                             k++;
                         }

                 serSum = average( core, k );
                 sigmaTemp = sigma;
                 sigma = stdDeviation( core, serSum, k );
                 for( int i = 0; i < k; i++ )
                    if( core[i] + 3 * sigma > serSum
                     || core[i] - 3 * sigma < serSum )
                        core[i] = (F32)serSum;
             }
             resDATA[NULL][j][i] = (F32)serSum;
         }

     return resDATA;
}

MATRIX<F32> * Processing::interpolation_Newton1( MATRIX<F32> * data )
{
    const U16 X = data->m_x, Y = data->m_y;
    MATRIX<F32> * resDATA = new MATRIX<F32>( Y, X );

    D64 z00, z11, z01, z10;
    z00 = z11 = z01 = z10 = 0.0;
    const S32 K = 10;   S32 k = 0;
    for( S32 j = 0, m = Y - 1; j < K; j++, m-- )
        for( S32 i = 0, n = X - 1; i < K; i++, n-- )
        {
            z00 += ((D64)data[NULL][j][i] - z00) / ((D64)k + 1.0);
            z11 += ((D64)data[NULL][m][n] - z11) / ((D64)k + 1.0);
            z01 += ((D64)data[NULL][m][i] - z01) / ((D64)k + 1.0);
            z10 += ((D64)data[NULL][j][n] - z10) / ((D64)k + 1.0);
            k++;
        }

    D64 firstDivX = z10 - z00;
    D64 firstDivY = z01 - z00;
    D64 secondDivXY = (z11 - z01) - (z10 - z00);

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            resDATA[NULL][j][i] = F32(z00);
            resDATA[NULL][j][i] += F32(firstDivX * (i + 1) / X);
            resDATA[NULL][j][i] += F32(firstDivY * (j + 1) / Y);
            resDATA[NULL][j][i] += F32(secondDivXY * (i + 1) * (j + 1) / X / Y);
        }

    return resDATA;
}

////////////////////////////////////
MATRIX<F32> * Processing::interpolation_Newton2( MATRIX<F32> * data )
{
    const U16 X = data->m_x, Y = data->m_y;
    MATRIX<F32> * resDATA = new MATRIX<F32>( Y, X );

    U16 halfX = X / 2;
    U16 halfY = Y / 2;

    D64 z00, z11, z22, z01, z10, z02, z20, z12, z21;
    z00 = z11 = z22 = z01 = z10 = z02 = z20 = z12 = z21 = 0.0;
    const S32 K = 4, halfK = K / 2;   S32 k = 0;
    for( S32 j = 0, m = Y - 1; j < K; j++, m-- )
        for( S32 i = 0, n = X - 1; i < K; i++, n-- )
        {
            z00 += ((D64)data[NULL][j][i] - z00) / ((D64)k + 1.0);
            z11 += ((D64)data[NULL][halfY-halfK+j][halfX-halfK+i] - z11) / ((D64)k + 1.0);
            z22 += ((D64)data[NULL][m][n] - z22) / ((D64)k + 1.0);
            z01 += ((D64)data[NULL][halfY-halfK+j][i] - z01) / ((D64)k + 1.0);
            z10 += ((D64)data[NULL][j][halfX-halfK+i] - z10) / ((D64)k + 1.0);
            z02 += ((D64)data[NULL][m][i] - z02) / ((D64)k + 1.0);
            z20 += ((D64)data[NULL][j][n] - z20) / ((D64)k + 1.0);
            z21 += ((D64)data[NULL][halfY-halfK+j][n] - z21) / ((D64)k + 1.0);
            z12 += ((D64)data[NULL][m][halfX-halfK+i] - z12) / ((D64)k + 1.0);
            k++;

            k++;
        }

    D64 firstDivX = z10 - z00;
    D64 firstDivY = z01 - z00;
    D64 secondDivXX = z20 - 2 * z10 + z00;
    D64 secondDivYY = z02 - 2 * z01 + z00;
    D64 secondDivXY = (z11 - z01) - (z10 - z00);
//  D64 thirdDivXXX = ((z30 - z20) - (z20 - z10)) - ((z20 - z10) - (z10 - z00));
//  D64 thirdDivYYY = ((z03 - z02) - (z02 - z01)) - ((z02 - z01) - (z01 - z00));
    D64 thirdDivXXX = ((z20 - z20) - (z20 - z10)) - ((z20 - z10) - (z10 - z00));
    D64 thirdDivYYY = ((z02 - z02) - (z02 - z01)) - ((z02 - z01) - (z01 - z00));
    D64 thirdDivXXY = ((z21 - z11) - (z11 - z01)) - ((z20 - z10) - (z10 - z00));
    D64 thirdDivXYY = ((z12 - z02) - (z11 - z01)) - ((z11 - z01) - (z10 - z00));

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            resDATA[NULL][j][i] = (F32)z00;
            resDATA[NULL][j][i] += F32(firstDivX * (i + 1) / halfX);
            resDATA[NULL][j][i] += F32(firstDivY * (j + 1) / halfY);
            resDATA[NULL][j][i] += F32(secondDivXX * (i + 1) * (i + 1 - halfX) / 2 / halfX / halfX);
            resDATA[NULL][j][i] += F32(secondDivYY * (j + 1) * (j + 1 - halfY) / 2 / halfY / halfY);
            resDATA[NULL][j][i] += F32(secondDivXY * (i + 1) * (j + 1) / halfX / halfY);
            resDATA[NULL][j][i] += F32(thirdDivXXX * (i + 1) * (i + 1 - halfX) * (i + 1 - X) / 6 / halfX / halfX / halfX );
            resDATA[NULL][j][i] += F32(thirdDivYYY * (j + 1) * (j + 1 - halfY) * (j + 1 - Y) / 6 / halfY / halfY / halfY );
            resDATA[NULL][j][i] += F32(thirdDivXXY * (i + 1) * (i + 1 - halfX) * (j + 1) / 2 / halfX / halfX / halfY );
            resDATA[NULL][j][i] += F32(thirdDivXYY * (i + 1) * (j + 1) * (j + 1 - halfY) / 2 / halfX / halfY / halfY );
        }

    F32 Z22 = resDATA[NULL][999][999];
    F32 Z11 = resDATA[NULL][499][499];
    F32 Z00 = resDATA[NULL][0][0];
    F32 Z20 = resDATA[NULL][0][999];
    F32 Z02 = resDATA[NULL][999][0];

   return resDATA;
}

MATRIX<F32> * Processing::interpolation_Newton3( MATRIX<F32> * data )
{
    const U16 X = data->m_x, Y = data->m_y;
    MATRIX<F32> * resDATA = new MATRIX<F32>( Y, X );

    U16 halfX = X / 2;
    U16 halfY = Y / 2;

    D64 z00, z11, z22, z01, z10, z02, z20, z12, z21;
    z00 = z11 = z22 = z01 = z10 = z02 = z20 = z12 = z21 = 0.0;
    const S32 K = MATRIX_SIZE * 0.1, halfK = K / 2;   S32 k = 0;
    for( S32 j = 0, m = Y - 1; j < K; j++, m-- )
        for( S32 i = 0, n = X - 1; i < K; i++, n-- )
        {
            z00 += ((D64)data[NULL][j][i] - z00) / ((D64)k + 1.0);
            z11 += ((D64)data[NULL][halfY-halfK+j][halfX-halfK+i] - z11) / ((D64)k + 1.0);
            z22 += ((D64)data[NULL][m][n] - z22) / ((D64)k + 1.0);
            z01 += ((D64)data[NULL][halfY-halfK+j][i] - z01) / ((D64)k + 1.0);
            z10 += ((D64)data[NULL][j][halfX-halfK+i] - z10) / ((D64)k + 1.0);
            z02 += ((D64)data[NULL][m][i] - z02) / ((D64)k + 1.0);
            z20 += ((D64)data[NULL][j][n] - z20) / ((D64)k + 1.0);
            z21 += ((D64)data[NULL][halfY-halfK+j][n] - z21) / ((D64)k + 1.0);
            z12 += ((D64)data[NULL][m][halfX-halfK+i] - z12) / ((D64)k + 1.0);
            k++;

            k++;
        }

    D64 firstDivX = z10 - z00;
    D64 firstDivY = z01 - z00;
    D64 secondDivXX = z20 - 2 * z10 + z00;
    D64 secondDivYY = z02 - 2 * z01 + z00;
    D64 secondDivXY = (z11 - z01) - (z10 - z00);
    D64 thirdDivXXX = ((z20 - z20) - (z20 - z10)) - ((z20 - z10) - (z10 - z00));
    D64 thirdDivYYY = ((z02 - z02) - (z02 - z01)) - ((z02 - z01) - (z01 - z00));
    D64 thirdDivXXY = ((z21 - z11) - (z11 - z01)) - ((z20 - z10) - (z10 - z00));
    D64 thirdDivXYY = ((z12 - z02) - (z11 - z01)) - ((z11 - z01) - (z10 - z00));

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X - j; i++ )
        {
            resDATA[NULL][j][i] = (F32)z00;
            resDATA[NULL][j][i] += F32(firstDivX * (i + 1) / halfX);
            resDATA[NULL][j][i] += F32(firstDivY * (j + 1) / halfY);
            resDATA[NULL][j][i] += F32(secondDivXX * (i + 1) * (i + 1 - halfX) / 2 / halfX / halfX);
            resDATA[NULL][j][i] += F32(secondDivYY * (j + 1) * (j + 1 - halfY) / 2 / halfY / halfY);
            resDATA[NULL][j][i] += F32(secondDivXY * (i + 1) * (j + 1) / halfX / halfY);
            resDATA[NULL][j][i] += F32(thirdDivXXX * (i + 1) * (i + 1 - halfX) * (i + 1 - X) / 6 / halfX / halfX / halfX );
            resDATA[NULL][j][i] += F32(thirdDivYYY * (j + 1) * (j + 1 - halfY) * (j + 1 - Y) / 6 / halfY / halfY / halfY );
            resDATA[NULL][j][i] += F32(thirdDivXXY * (i + 1) * (i + 1 - halfX) * (j + 1) / 2 / halfX / halfX / halfY );
            resDATA[NULL][j][i] += F32(thirdDivXYY * (i + 1) * (j + 1) * (j + 1 - halfY) / 2 / halfX / halfY / halfY );
        }

    MATRIX<F32> * dataTR = transpose( data );

    k = 0;
    z00 = z11 = z22 = z01 = z10 = z02 = z20 = z12 = z21 = 0.0;
    for( S32 j = 0, m = Y - 1; j < K; j++, m-- )
        for( S32 i = 0, n = X - 1; i < K; i++, n-- )
        {
            z00 += ((D64)dataTR[NULL][j][i] - z00) / ((D64)k + 1.0);
            z11 += ((D64)dataTR[NULL][halfY-halfK+j][halfX-halfK+i] - z11) / ((D64)k + 1.0);
            z22 += ((D64)dataTR[NULL][m][n] - z22) / ((D64)k + 1.0);
            z01 += ((D64)dataTR[NULL][halfY-halfK+j][i] - z01) / ((D64)k + 1.0);
            z10 += ((D64)dataTR[NULL][j][halfX-halfK+i] - z10) / ((D64)k + 1.0);
            z02 += ((D64)dataTR[NULL][m][i] - z02) / ((D64)k + 1.0);
            z20 += ((D64)dataTR[NULL][j][n] - z20) / ((D64)k + 1.0);
            z21 += ((D64)dataTR[NULL][halfY-halfK+j][n] - z21) / ((D64)k + 1.0);
            z12 += ((D64)dataTR[NULL][m][halfX-halfK+i] - z12) / ((D64)k + 1.0);
            k++;

            k++;
        }

    delete dataTR;

    firstDivX = z10 - z00;
    firstDivY = z01 - z00;
    secondDivXX = z20 - 2 * z10 + z00;
    secondDivYY = z02 - 2 * z01 + z00;
    secondDivXY = (z11 - z01) - (z10 - z00);
    thirdDivXXX = ((z20 - z20) - (z20 - z10)) - ((z20 - z10) - (z10 - z00));
    thirdDivYYY = ((z02 - z02) - (z02 - z01)) - ((z02 - z01) - (z01 - z00));
    thirdDivXXY = ((z21 - z11) - (z11 - z01)) - ((z20 - z10) - (z10 - z00));
    thirdDivXYY = ((z12 - z02) - (z11 - z01)) - ((z11 - z01) - (z10 - z00));

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X - j; i++ )
        {
            resDATA[NULL][Y-j-1][X-i-1] = (F32)z00;
            resDATA[NULL][Y-j-1][X-i-1] += F32(firstDivX * (i + 1) / halfX);
            resDATA[NULL][Y-j-1][X-i-1] += F32(firstDivY * (j + 1) / halfY);
            resDATA[NULL][Y-j-1][X-i-1] += F32(secondDivXX * (i + 1) * (i + 1 - halfX) / 2 / halfX / halfX);
            resDATA[NULL][Y-j-1][X-i-1] += F32(secondDivYY * (j + 1) * (j + 1 - halfY) / 2 / halfY / halfY);
            resDATA[NULL][Y-j-1][X-i-1] += F32(secondDivXY * (i + 1) * (j + 1) / halfX / halfY);
            resDATA[NULL][Y-j-1][X-i-1] += F32(thirdDivXXX * (i + 1) * (i + 1 - halfX) * (i + 1 - X) / 6 / halfX / halfX / halfX );
            resDATA[NULL][Y-j-1][X-i-1] += F32(thirdDivYYY * (j + 1) * (j + 1 - halfY) * (j + 1 - Y) / 6 / halfY / halfY / halfY );
            resDATA[NULL][Y-j-1][X-i-1] += F32(thirdDivXXY * (i + 1) * (i + 1 - halfX) * (j + 1) / 2 / halfX / halfX / halfY );
            resDATA[NULL][Y-j-1][X-i-1] += F32(thirdDivXYY * (i + 1) * (j + 1) * (j + 1 - halfY) / 2 / halfX / halfY / halfY );
        }


    return resDATA;
}

////////////////////////////////////
MATRIX<F32> * Processing::aproximation_OLSq2( MATRIX<F32> * data )
{
    const U16 X = data->m_x, Y = data->m_y;
    MATRIX<F32> * resDATA = new MATRIX<F32>( Y, X );

    // f(x,y) = a0 + a1 * x*x + a2 * y*y + a3 * x*y + a4 * x + a5 * y
    const S32 N = 10;        // полином 3-й степени, 2-мерный случай
    const S32 M = 1000000;  // всего уравнний
    const S32 K = 10;      // размер пикселя K*K (для сред значения)

    F32 lM = sqrt( (F32)M );
    F32 stepX = X / lM;
    F32 stepY = Y / lM;

    MathMatrix XY( M, N );
    MathMatrix Z( M, 1 );

    S32 m = 0;
    for( S32 j = 0; j < Y; j+= stepY )
        for( S32 i = 0; i < X; i+= stepX )
        {
            S32 k = 0;  D64 dataPix = 0.0;
            for( S32 q = j - K/2; q < j + K/2; q++ )
                for( S32 p = i - K/2; p < i + K/2; p++ )
                    if( p > 0 && p < X && q > 0 && q < Y )
                    {
                        dataPix += ((D64)(*data)[q][p] - dataPix) / ((D64)k + 1.0);
                        k++;
                    }
            Z[m][0] = dataPix;      // Заполнение N-елементов значениями f(x,y)
            XY[m][0] = 1.0f;        // Заполнение N-елементов матрицы по X
            XY[m][1] = i * i * i;
            XY[m][2] = j * j * j;
            XY[m][3] = i * i * j;
            XY[m][4] = i * j * j;
            XY[m][5] = i * i;
            XY[m][6] = j * j;
            XY[m][7] = i * j;
            XY[m][8] = i;
            XY[m][9] = j;

            m++;
        }

    MathMatrix XYtr = XY;	XYtr.toTranspose(); // МНК
    MathMatrix XYtrXYinv = XYtr * XY;	XYtrXYinv.toInvers();
    MathMatrix A = XYtrXYinv * XYtr * Z;

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            (*resDATA)[j][i] = A[0][0] + A[1][0] * i*i*i + A[2][0] * j*j*j;
            (*resDATA)[j][i] += A[3][0] * i*i*j + A[4][0] * i*j*j + A[5][0] * i*i;
            (*resDATA)[j][i] += A[6][0] * j*j + A[7][0] * i*j + A[8][0] * i + A[9][0] * j;
        }

    return resDATA;
}

////////////////////////////////////
MATRIX<F32> * Processing::filter_justFilter( MATRIX<F32> * data, U16 coreSize )
{
    const U16 X = data->m_x, Y = data->m_y;
    MATRIX<F32> * resDATA = new MATRIX<F32>( Y, X );

    const U16 cSizeL = coreSize * coreSize;
    const S32 halfL = S32(coreSize / 2.0 + 0.5);

    const S32 N = 1000;
    vector<F32> mainCore( N );
    for( int i = 0; i < N; i++ )
    {
        S32 itX = rand() % X;
        S32 itY = rand() % Y;
        mainCore[i] = data[NULL][itY][itX];
    }

    D64 mainSerSum = average( mainCore, N );

    const D64 dev = 0.6;

    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            vector<F32> core( cSizeL );

            U16 k = 0;
            for( S32 m = 0, q = -halfL; m < coreSize; m++, q++ )
                for( S32 n = 0, p = -halfL; n < coreSize; n++, p++ )
                    if( i+p >= 0 && i+p < X && j+q >= 0 && j+q < Y )
                    {
                        core[k] = data[NULL][j+q][i+p];
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

            resDATA[NULL][j][i] = (F32)aver;
        }

    return resDATA;
}


MATRIX<F32> * Processing::filter_InclinedFilter( MATRIX<F32> * data, U16 coreSize )
{
    const U16 X = data->m_x, Y = data->m_y;
    MATRIX<F32> * resDATA = new MATRIX<F32>( Y, X );

    MATRIX<bool> chker( Y, X );
    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
            chker[j][i] = true;

    S32 lSize = coreSize;
    vector<F32> core( lSize );
    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            for( S32 k = 0; k < lSize; k++ )
            {
                if( i > lSize )
                    if( chker[j][i-lSize-1+k] )
                        core[k] = (*data)[j][i-lSize-1+k];
                else
                {
                    if( chker[j][i+1+k] )
                        core[k] = (*data)[j][i+1+k];
                }
            }

            D64 ser = average( core, coreSize );
            D64 sigma = stdDeviation( core, ser, coreSize );

            if( (*data)[j][i] < ser * 3 * sigma )
            {
                 (*resDATA)[j][i] = (*data)[j][i];
            }
            else
            {
                chker[j][i] = false;
                (*resDATA)[j][i] = 0;
            }
        }

    return resDATA;
}


 MATRIX<F32> * Processing::normalization( MATRIX<F32> * data, F32 norm )
 {
     const U16 X = data->m_x, Y = data->m_y;
     MATRIX<F32> * resDATA = new MATRIX<F32>( Y, X );


     F32 min = numeric_limits<F32>::max();
     F32 max = numeric_limits<F32>::min();
     for( S32 j = 0; j < Y; j++ )
         for( S32 i = 0; i < X; i++ )
         {
             if( (*data)[j][i] < min )  min = (*data)[j][i];
             if( (*data)[j][i] > max )  max = (*data)[j][i];
         }

     for( S32 j = 0; j < Y; j++ )
         for( S32 i = 0; i < X; i++ )
         {
            (*resDATA)[j][i] = ((*data)[j][i] - min) / (max - min);
            (*resDATA)[j][i] *= norm;
         }

     return resDATA;
 }

////////////////////////////////////
////////////////////////////////////
vector<Spot> Processing::searcher( MATRIX<F32> * data )
{

}

////////////////////////////////////
////////////////////////////////////

Catalog * Processing::dedectedGlobularClasters( Catalog * inCat )
{
    inCat->calculateMinMaxParameters();

    U32 X = MATRIX_SIZE;
    U32 Y = MATRIX_SIZE;

    const D64 scaleX = (D64)X / (inCat->getMaxRa() - inCat->getMinRa());
    const D64 scaleY = (D64)Y / (inCat->getMaxDec() - inCat->getMinDec());

    MATRIX<F32> * DATA = new MATRIX<F32>( Y, X );
    U32 N = inCat->getObjectCount();
    for( U32 i = 0; i < N; i++ )
    {
        U32 x = U32((inCat->getObject( i ).getRa() - inCat->getMinRa()) * scaleX);
        U32 y = U32((inCat->getObject( i ).getDec() - inCat->getMinDec()) * scaleY);
        if( x >= X )    x = X - 1;
        if( y >= Y )    y = Y - 1;
        DATA[NULL][y][x]++;
    }

    Fits fits;

    fits.setFile( "data1.fits" );
    fits.setDATA( DATA );
    fits.writeFitsFile();

    MATRIX<F32> *interDATA = convolution_Gauss( DATA, KERNEL_SIZE/2 );
    delete DATA;    DATA = interDATA;

    fits.setFile( "data3.fits" );
    fits.setDATA( DATA );
    fits.writeFitsFile();

    interDATA = aproximation_OLSq2( DATA );
    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            DATA[NULL][j][i] -= interDATA[NULL][j][i];
            if( DATA[NULL][j][i] < 0 )  DATA[NULL][j][i] = 0;
            interDATA[NULL][j][i] *= 1000;
        }

    fits.setFile( "dataINTER.fits" );
    fits.setDATA( interDATA );
    fits.writeFitsFile();

    delete interDATA;

    interDATA = normalization( DATA, 10 );
    delete DATA;
    DATA = interDATA;

    fits.setFile( "dataNORM.fits" );
    fits.setDATA( DATA );
    fits.writeFitsFile();

    interDATA = filter_InclinedFilter( DATA, KERNEL_SIZE );
    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
        {
            DATA[NULL][j][i] -= interDATA[NULL][j][i];
            if( DATA[NULL][j][i] < 0 )  DATA[NULL][j][i] = 0;
            interDATA[NULL][j][i] *= 1000;
        }

    fits.setFile( "data2.fits" );
    fits.setDATA( DATA );
    fits.writeFitsFile();

    fits.setFile( "data2_Fiter.fits" );
    fits.setDATA( interDATA );
    fits.writeFitsFile();

    delete interDATA;

    interDATA = convolution_Gauss( DATA, KERNEL_SIZE/2 );
    delete DATA;    DATA = interDATA;

    fits.setFile( "data3.fits" );
    fits.setDATA( DATA );
    fits.writeFitsFile();

    vector<Dot> clasters = clasteroider(  DATA );
    Catalog * resGlobularClasters = new Catalog;
    for( U32 i = 0; i < clasters.size(); i++ )
    {
        D64 ra = inCat->getMinRa() + (D64)clasters[i].x / scaleX;
        D64 dec = inCat->getMinDec() + (D64)clasters[i].y / scaleY;
        D64 size = clasters[i].size;

        CelestialBody claster;
        claster.init( i + 1, ra, dec, size, 0.0, 0.0, 0.0, CelestialBodyType_Claster );
        resGlobularClasters->addObject( claster );
    }

    return resGlobularClasters;
}
////////////////////////////////////
////////////////////////////////////

vector<Dot> Processing::checkingAdjacentCells( MATRIX<U16> * data, Dot dot, U16 minValue )
{
    vector<Dot> res( 4 );               //какашкаКод
    U16 x = dot.x; U16 y = dot.y;

    S32 z1, z2, z3, z4;
    z1 = x - 1; if( x - 1 < 0 ) z1 = x;
    z2 = x + 1; if( x + 1 >= data->m_x ) z2 = x;
    z3 = y - 1; if( y - 1 < 0 ) z3 = y;
    z4 = y + 1; if( y + 1 >= data->m_y ) z4 = y;

    res[0].x = x; res[0].y = z3;
    res[1].x = x; res[1].y = z4;
    res[2].x = z1; res[2].y = y;
    res[3].x = z2; res[3].y = y;
    if( data[NULL][z3][x] > minValue )res[0].size = 1;
    if( data[NULL][z4][x] > minValue )res[1].size = 1;
    if( data[NULL][y][z1] > minValue )res[2].size = 1;
    if( data[NULL][y][z2] > minValue )res[3].size = 1;

    return res;
}

vector<Dot> Processing::recursChecking( MATRIX<U16> * data, Dot dot, U16 minValue, vector<Dot> adjacentDots )
{
    vector<Dot> res;
    for( U32 k = 0; k < adjacentDots.size(); k++ )
        if( adjacentDots[k].size )
        {
            data[NULL][adjacentDots[k].y][adjacentDots[k].x] = minValue;
            res.emplace_back( adjacentDots[k] );
            vector<Dot> tempAdjacentDots = checkingAdjacentCells( data, adjacentDots[k], minValue );
            vector<Dot> tempRes = recursChecking( data, adjacentDots[k], minValue, tempAdjacentDots );
            if( !tempRes.empty() )
            {
/*
                int aa = res.size();
                int bb = tempRes.size();
                int cc = aa + bb;

                m_file.open( "logg.txt", ios_base::app );
                m_file << aa << " " << bb << " " << cc << endl;
                m_file.close();
*/
                res.reserve( res.size() + tempRes.size() );
                res.insert( res.end(), tempRes.begin(), tempRes.end() );
            }
        }
    return res;
}

////////////////////////////////////
////////////////////////////////////

vector<Dot> Processing::clasteroider( MATRIX<F32> * data )
{
    vector<Dot> spots;

    U32 X = data->m_x;
    U32 Y = data->m_y;
    U32 iterr = 0;
    MATRIX<U16> DATA( Y, X );
    for( U32 j = 0; j < Y; j++ )
        for( U32 i = 0; i < X; i++ )
        {
            DATA[j][i] = (U16)data[NULL][j][i];
             if( DATA[j][i] )
                 iterr++;
        }

 /*   if( iterr > X * Y * 0.001 )
        return spots;*/

    for( U32 j = 0; j < Y; j++ )
        for( U32 i = 0; i < X; i++ )
            if( DATA[j][i] )
            {
                Dot dot; dot.x = i; dot.y = j; dot.size = 1;
                vector<Dot> adjacentDots = checkingAdjacentCells( &DATA, dot, 0 );
                vector<Dot> notNullDots = recursChecking( &DATA, dot, 0, adjacentDots );
                notNullDots.emplace_back( dot );

                U16 xMin = DATA.m_x, xMax = 0;
                U16 yMin = DATA.m_y, yMax = 0;
                for( U32 k = 0; k < notNullDots.size(); k++ )
                {
                    if( notNullDots[k].x < xMin )  xMin = notNullDots[k].x;
                    if( notNullDots[k].x > xMax )  xMax = notNullDots[k].x;
                    if( notNullDots[k].y < yMin )  yMin = notNullDots[k].y;
                    if( notNullDots[k].y > yMax )  yMax = notNullDots[k].y;
                }

                Dot spot;
                spot.size = xMax - xMin;
                spot.x = (xMax - xMin) / 2 + xMin;
                spot.y = (yMax - yMin) / 2 + yMin;
                spots.emplace_back( spot );



                if( spots.size() >= 25 ) //rfrfirjl
                {
                    fstream f;
                    f.open( PATHTOFILE + "pmaQParts.txt",std::ios_base::app );
                    f << FILENAME << "\t" << spots.size() << endl;

                     return spots;
                }


            }

    if( spots.size() > 5 )
    {
        fstream f;
        f.open( "pmaQParts.txt",std::ios_base::app );
        f << FILENAME << "\t" << spots.size() << endl;
    }

    return spots;
}


void Processing::intrFunction( Catalog * GCcat, string fileNameCat )
{
    fstream file;
    file.open( fileNameCat.c_str(), ios_base::in );

    string fileStr;

    S32 N = GCcat->getObjectCount();
    Catalog * GCrs = new Catalog[N];

    S32 S = 3;  //rad

    string header;
    getline( file, header );

    U64 ID = 0;
    while( getline( file, fileStr ) )
    {
        STRS w = divideByWordsSrtings( fileStr, 16 );
        D64 ra = stod( w[0] ), dec = stod( w[1] );

        for( S32 i = 0; i < N; i++ )
        {
            D64 gcRa = GCcat->getObject( i ).getRa();
            D64 gcDec = GCcat->getObject( i ).getDec();
            D64 gcRad = 0.0033;

            if( ra > gcRa - S * gcRad && ra < gcRa + S * gcRad &&
                dec > gcDec - S * gcRad && dec < gcDec + S * gcRad )
            {
                CelestialBody body;
                body.init( ID, ra, dec, 0.0, stod( w[2] ), stod( w[3] ), stod( w[6] ), CelestialBodyType_Star );
                GCrs[i].addObject( body );
            }
        }

        ID++;
        if( !(ID % 100000) )
            cout << ID << "..." << endl;
    }

    cout << ID << "." << endl;
    cout << "Writing..." << endl;

    for( S32 i = 0; i < N; i++ )
    {
        GCrs[i].writeToFile( "pmaGCcat_" + to_string( i ) + ".txt" );
        Fits * fits = catalogueToFits( &GCrs[i] );
        fits->setFile( "pmaGCcat_" + to_string( i ) + ".fits" );
        fits->writeFitsFile();
        delete fits;

        cout << i << "..." << endl;
    }

}

/*
Catalog * Processing::createGlobularClaster( U32 r, U32 n, D64 quality )
{
    Catalog * claster = new Catalog();

    D64 ra0, dec0, prx0;
    ra0 = dec0 = prx0 = 0.0;

    D64 raSig =  r / 10.0 * 1 + quality;
    D64 decSig = r / 10.0 * 1 + quality;
    D64 prxSig = r / 10.0 * quality;

    for( U32 i = 0; i < n; i++ )
    {
        D64 ra  = rand() % r;    if( rand() % 2 )    ra =  -ra;
        D64 dec = rand() % r;    if( rand() % 2 )    dec = -dec;
        D64 prx = rand() % r;    if( rand() % 2 )    prx = -prx;
        D64 raDis  = exp( pow( ra  - ra0,  2 ) / 2 / pow( raSig,  2.0 ) );
        D64 decDis = exp( pow( dec - dec0, 2 ) / 2 / pow( decSig, 2.0 ) );
        D64 prxDis = exp( pow( prx - prx0, 2 ) / 2 / pow( prxSig, 2.0 ) );
        D64 distribution2D = 1.0 / raDis / decDis / prxDis;
 //     distribution2D /= pow( 2 * 3.141592, 3.0 / 2.0 ) * raSig * decSig * prxSig;

        D64 distance = sqrt( pow( ra, 2.0 ) + pow( dec, 2.0 ) + pow( prx, 2.0 ) );
        D64 sigma = sqrt( pow( raSig, 2.0 ) + pow( decSig, 2.0 ) + pow( prxSig, 2.0 ) );

        if( rand() % 100 < distribution2D * 100 && distance < 5 * sigma )
        {
            CelestialBody star;
            star.init( 	i + 1, ra, dec, prx, 0, 0, 0, CelestialBodyType_Star );
            claster->addObject( star );
        }
    }

    return claster;
}

Catalog * Processing::dedectedGlobularClasters_method1( Catalog * inCat )
{
    const U32 scale = 1000;

    inCat->calculateMinMaxParameters();
    U32 X = U32((inCat->getMaxRa() - inCat->getMinRa()) * scale + 0.5) + 1;
    U32 Y = U32((inCat->getMaxDec() - inCat->getMinDec()) * scale + 0.5) + 1;

    MATRIX<U16> DATA( Y, X );
    U64 N = inCat->getObjectCount();
    for( U32 i = 0; i < N; i++ )
    {
        U32 x = U32((inCat->getObject( i )->getRa() - inCat->getMinRa()) * scale);
        U32 y = U32((inCat->getObject( i )->getDec() - inCat->getMinDec()) * scale);
        if( x >= X )    x = X - 1;
        if( y >= Y )    y = Y - 1;
        DATA[y][x]++;
    }

    Fits fits;
    fits.setFile( "data.fits" );
    fits.setDATA( &DATA );
    fits.writeFitsFile();

    return inCat;
}

Catalog * Processing::dedectedGlobularClasters_method2( Catalog * inCat )
{
     inCat->calculateMinMaxParameters();

//  D64 delta = 0.000277777;    // 1сек дуги
    D64 delta = 0.001666666;    // 6сек дуги
    fstream fff( "data.txt", ios_base::out );

    D64 a = inCat->getMinRa();
    D64 b = a + delta;
    D64 c = inCat->getMinDec();
    D64 d = c + delta;

    while( true )
    {
        fff << inCat->getSubCatalog( a, b, c, d )->getObjectCount() << "\t"
            << a + delta / 2 << "\t" << c + delta / 2 << endl;

        if( b >= inCat->getMaxRa() )
        {
            if( d >= inCat->getMaxDec() )
                break;
            a = inCat->getMinRa() - delta;
            b = a + delta;
            c += delta;
            d += delta;
        }
        a += delta;
        b += delta;
    }

    return inCat;
}

Catalog * Processing::dedectedGlobularClasters_method3( Catalog * inCat )
{
    cout << "Start Convolution... ";

    const U32 scale = 1000;

    inCat->calculateMinMaxParameters();
    U32 X = U32((inCat->getMaxRa() - inCat->getMinRa()) * scale + 0.5) + 1;
    U32 Y = U32((inCat->getMaxDec() - inCat->getMinDec()) * scale + 0.5) + 1;


    U16 ** DATA;
    DATA = new U16*[Y];
    for( U32 j = 0; j < Y; j++ )
        DATA[j] = new U16[X];
    for( U32 j = 0; j < Y; j++ )
        for( U32 i = 0; i < X; i++ )
            DATA[j][i] = 0;

    U64 N = inCat->getObjectCount();
    for( U32 i = 0; i < N; i++ )
    {
        U32 x = U32((inCat->getObject( i )->getRa() - inCat->getMinRa()) * scale);
        U32 y = U32((inCat->getObject( i )->getDec() - inCat->getMinDec()) * scale);
        if( x >= X )    x = X - 1;
        if( y >= Y )    y = Y - 1;
        DATA[y][x]++;
    }

    U16 ** DATAb;
    DATAb = new U16*[Y];
    for( U32 j = 0; j < Y; j++ )
        DATAb[j] = new U16[X];
    for( U32 j = 0; j < Y; j++ )
        for( U32 i = 0; i < X; i++ )
            DATAb[j][i] = DATA[j][i];

    DATA = GaussFilter( DATAb, Y, X, 5, 5 );

    fstream fff( "data.txt", ios_base::out );
    for( U32 j = 0; j < Y; j++ )
        for( U32 i = 0; i < X; i++ )
            fff << i << "\t" << j << "\t" << DATA[j][i] << endl;

    return inCat;
}

Catalog * Processing::dedectedGlobularClasters_method4( Catalog * inCat )
{
    const U32 scale = 10000;

    inCat->calculateMinMaxParameters();
    U32 X = U32((inCat->getMaxRa() - inCat->getMinRa()) * scale + 0.5) + 1;
    U32 Y = U32((inCat->getMaxDec() - inCat->getMinDec()) * scale + 0.5) + 1;

    MATRIX<U32> DATA( Y, X );
    U64 N = inCat->getObjectCount();
    for( U32 i = 0; i < N; i++ )
    {
        U32 x = U32((inCat->getObject( i )->getRa() - inCat->getMinRa()) * scale);
        U32 y = U32((inCat->getObject( i )->getDec() - inCat->getMinDec()) * scale);
        if( x >= X )    x = X - 1;
        if( y >= Y )    y = Y - 1;
        DATA[y][x]++;
    }

    const U16 kSizeX = 15;
    const U16 kSizeY = 15;
    const D64 SigX = kSizeX / 5.0;
    const D64 SigY = kSizeY / 5.0;

    MATRIX<D64> KERNEL( kSizeY, kSizeX );
    D64 halfX = kSizeX / 2.0;
    D64 halfY = kSizeY / 2.0;

    D64 sumKernel = 0;
    for( U32 m = 0; m < kSizeY; m++ )
        for( U32 n = 0; n < kSizeX; n++ )
        {
            D64 constitX = exp( pow( (D64)n  - halfX + 0.5,  2.0 ) / 2 / pow( SigX,  2.0 ) );
            D64 constitY = exp( pow( (D64)m  - halfY + 0.5,  2.0 ) / 2 / pow( SigY,  2.0 ) );
            KERNEL[m][n] = 1.0 / constitX / constitY / (2 * PI * SigX * SigY);
            sumKernel += KERNEL[m][n];
        }

    for( U32 m = 0; m < kSizeY; m++ )
        for( U32 n = 0; n < kSizeX; n++ )
            KERNEL[m][n] /= sumKernel;      //ДоНормировка

    MATRIX<D64> newDATA( Y, X );

    for( U32 j = 0; j < Y; j++ )
        for( U32 i = 0; i < X; i++ )
        {
            newDATA[j][i] = 0.0;
            S32 a = i - halfX; if( a <= 0 )  a = 0;
            S32 b = i + halfX; if( b >= X )  b = X - 1;
            S32 c = j - halfY; if( c <= 0 )  c = 0;
            S32 d = j + halfY; if( d >= Y )  d = Y - 1;
            for( S32 m = 0, q = -halfY; m < kSizeY; m++, q++ )
                for( S32 n = 0, p = -halfX; n < kSizeX; n++, p++ )
                    if( i+p >= a && i+p <= b && j+q >= c && j+q <= d )
                        newDATA[j][i] += (D64)DATA[j+q][i+p] * KERNEL[m][n];
        }

  //MATRIX<D64> * new2DATA = medianFilter_iteratedMeth( &newDATA );
    MATRIX<U16> new3DATA( Y, X );
    for( U32 j = 0; j < Y; j++ )
        for( U32 i = 0; i < X; i++ )
            new3DATA[j][i] =  (U16)newDATA[j][i];

    vector<Dot> clasters = clasteroider(  &new3DATA );

    fstream fff( "daata.txt", ios_base::app );
    for( U32 i = 0; i < clasters.size(); i++ )
    {
        D64 ra = inCat->getMinRa() + (D64)clasters[i].x / scale;
        D64 dec = inCat->getMinDec() + (D64)clasters[i].y / scale;
        D64 size = ((D64)clasters[i].size + (D64)clasters[i].size / 2.0) / scale;
        fff << i << "\t" << ra << "\t" << dec << "\t" << size << endl;
    }


    Fits fits;
    fits.setFile( "data.fits" );
    fits.setDATA( &new3DATA );
    fits.writeFitsFile();

    return inCat;
}


void Processing::dedectedGlobularClasters_method5()
{
    Fits fitsInput;
    fitsInput.setFile( "D:\\.fits_files\\pixel_origin12.fits" );
    fitsInput.readFitsFile();
    MATRIX<U16> * DATA = fitsInput.getDATA();

    U32 X = DATA->m_x;
    U32 Y = DATA->m_y;
    for( S32 j = 0; j < Y; j++ )
        for( S32 i = 0; i < X; i++ )
            if( DATA[NULL][j][i] >= 32768 ) DATA[NULL][j][i] -= 32768;

    MATRIX<U16> * new2DATA = medianFilter_iteratedMeth( DATA );

    Fits fits2;
    fits2.setFile( "data_median.fits" );
    fits2.setDATA( new2DATA );
    fits2.writeFitsFile();

    const U16 kSizeX = 15;
    const U16 kSizeY = 15;
    const D64 SigX = kSizeX / 5.0;
    const D64 SigY = kSizeY / 5.0;

    MATRIX<D64> KERNEL( kSizeY, kSizeX );
    D64 halfX = kSizeX / 2.0;
    D64 halfY = kSizeY / 2.0;

    D64 sumKernel = 0;
    for( U32 m = 0; m < kSizeY; m++ )
        for( U32 n = 0; n < kSizeX; n++ )
        {
            D64 constitX = exp( pow( (D64)n  - halfX + 0.5,  2.0 ) / 2 / pow( SigX,  2.0 ) );
            D64 constitY = exp( pow( (D64)m  - halfY + 0.5,  2.0 ) / 2 / pow( SigY,  2.0 ) );
            KERNEL[m][n] = 1.0 / constitX / constitY / (2 * PI * SigX * SigY);
            sumKernel += KERNEL[m][n];
        }

    for( U32 m = 0; m < kSizeY; m++ )
        for( U32 n = 0; n < kSizeX; n++ )
            KERNEL[m][n] /= sumKernel;     //ДоНормировка

    MATRIX<D64> newDATA( Y, X );

    for( U32 j = 0; j < Y; j++ )
        for( U32 i = 0; i < X; i++ )
        {
            newDATA[j][i] = 0.0;
            S32 a = i - halfX; if( a <= 0 )  a = 0;
            S32 b = i + halfX; if( b >= X )  b = X - 1;
            S32 c = j - halfY; if( c <= 0 )  c = 0;
            S32 d = j + halfY; if( d >= Y )  d = Y - 1;
            for( S32 m = 0, q = -halfY; m < kSizeY; m++, q++ )
                for( S32 n = 0, p = -halfX; n < kSizeX; n++, p++ )
                    if( i+p >= a && i+p <= b && j+q >= c && j+q <= d )
                        newDATA[j][i] += (O64)new2DATA[NULL][j+q][i+p] * KERNEL[m][n];
        }

    fstream fff( "data.txt", ios_base::out );
    for( U32 j = 0; j < Y; j++ )
        for( U32 i = 0; i < X; i++ )
            fff << i << "\t" << j << "\t" << newDATA[j][i] << endl;

//    MATRIX<D64> * new2DATA = medianFilter_iteratedMeth( &newDATA );
    MATRIX<U16> new3DATA( Y, X );
    for( U32 j = 0; j < Y; j++ )
        for( U32 i = 0; i < X; i++ )
            new3DATA[j][i] =  (U16)newDATA[j][i];



    Fits fits;
    fits.setFile( "data.fits" );
    fits.setDATA( &new3DATA );
    fits.writeFitsFile();
}*/


Fits * Processing::catalogueToFits( Catalog * inCat )
{

    if( inCat->getObjectCount() )
    {

        inCat->calculateMinMaxParameters();

        const D64 scale = 0.00012; // 1px = 0.432sec

        U32 X = (inCat->getMaxRa() - inCat->getMinRa()) / scale + 0.5;
        U32 Y = (inCat->getMaxDec() - inCat->getMinDec()) / scale + 0.5;

        MATRIX<F32> * DATA = new MATRIX<F32>( Y, X );
        U32 N = inCat->getObjectCount();
        for( U32 i = 0; i < N; i++ )
        {
            U32 x = U32((inCat->getObject( i ).getRa() - inCat->getMinRa()) / scale);
            U32 y = U32((inCat->getObject( i ).getDec() - inCat->getMinDec()) / scale);
            if( x >= X )    x = X - 1;
            if( y >= Y )    y = Y - 1;
            DATA[NULL][y][x]++;
        }

        Fits * fits = new Fits();
        fits->setDATA( DATA );

        return fits;
    }
    else
    {
        MATRIX<F32> * DATA = new MATRIX<F32>( 1, 1 );
        Fits * fits = new Fits();
        fits->setDATA( DATA );
        return fits;
    }
}
