////////////////////////////////////
//class MathMatrix : matrix<float>
//
//C++98/03 (noLib)
//incl: matrix.h
//
//
//byTRiOLD -l-      04.18 upd: 05.18
////////////////////////////////////
#pragma once

#include "matrix.h"

////////////////////////////////////
class MathMatrix : public matrix<float>
{
public:
    MathMatrix()
        : matrix() {}

    MathMatrix( const int rowsY, const int colmsX )
        : matrix( rowsY, colmsX ) {}

private:
    const static int MAX_QSIZE = 16;	// 256 el

    ///////////////
public:
    bool toTranspose()
    {
        if( this->isEmpty() )
            return false;

        int X = this->getSizeX();
        int Y = this->getSizeY();

        MathMatrix newM( X, Y );
        for( int j = 0; j < Y; j++ )
            for( int i = 0; i < X; i++ )
                newM.at( i, j ) = this->at( j, i );

        this->create( newM );
        return true;
    }

    bool toInvers() // by fullGauss method
    {
        if(	this->isEmpty() )
            return false;

        int X = this->getSizeX();
        int Y = this->getSizeY();
        if( X != Y || X > MAX_QSIZE )
            return false;
        int S = X;
        MathMatrix newM( S, S );
        MathMatrix tempM( *this );
        for( int i = 0; i < S; i++ )
            newM[i][i] = 1.0f;

        for( int r = 0; r < S; r++ )
            if( this->at( r, r ) )
            {
                float norm = this->at( r, r );
                if( norm )
                    for( int c = 0; c < S; c++ )
                    {
                        this->at( r, c ) /= norm;
                        newM.at( r, c ) /= norm;
                    }
                for( int q = r + 1; q <= S; q++ )
                {
                    norm = this->at( q, r );
                    if( norm )
                        for( int p = 0; p < S; p++ )
                        {
                            this->at( q, p ) -= this->at( r, p ) * norm;
                            newM.at( q, p ) -= newM.at( r, p ) * norm;
                        }
                }

                for( int q = r - 1; q >= 0; q-- )
                {
                    norm = this->at( q, r );
                    if( norm )
                        for( int p = 0; p < S; p++ )
                        {
                            this->at( q, p ) -= this->at( r, p ) * norm;
                            newM.at( q, p ) -= newM.at( r, p ) * norm;
                        }
                }
            }
            else
            {
                bool check = false;
                for( int m = r; m < S; m++ )
                    if( this->at( m, r ) )
                    {
                        float * buffRow1 = new float[S];
                        float * buffRow2 = new float[S];
                        for( int n = 0; n < S; n++ )
                        {
                            buffRow1[n] = this->at( m, n );
                            buffRow2[n] = newM.at( m, n );
                            this->at( m, n ) = this->at( r, n );
                            newM.at( m, n ) = newM.at( r, n );
                            this->at( r, n ) = buffRow1[n];
                            newM.at( r, n ) = buffRow2[n];
                        }
                        delete [] buffRow1;
                        delete [] buffRow2;

                        check = true;
                        break;
                    }
                if( !check )
                {
                    this->create( tempM );
                    return false;
                }
                r--;
            }

        this->create( newM );
        return true;
    }

    MathMatrix operator * ( MathMatrix &M2 )
    {
        int I = this->getSizeX();	int J = this->getSizeY();
        int N = M2.getSizeX();		int M = M2.getSizeY();

        MathMatrix res;
        if( I != M )	return res;

        int X = N;	int Y = J; int K = M;
        res.create( Y, X, false );

        for( int j = 0; j < Y; j++ )
            for( int i = 0; i < X; i++ )
                for( int k = 0; k < K; k++ )
                    res.at( j, i ) += this->at( j, k ) * M2.at( k, i );

        return res;
    }
};
