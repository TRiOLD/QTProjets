#ifndef PICTUREOBJECT_H
#define PICTUREOBJECT_H

#include "Improvements.h"
#include "matrix.h"

class PictureObject
{
public:
    PictureObject();

public:
    struct Dot
    {
        Dot()
        : X( 0 ), Y( 0 ), Z( 0 ){}
        Dot( S32 x, S32 y, S32 z )
        : X( x ), Y( y ), Z( z ){}
        S32 X, Y, Z;
    };

private:
    S32 m_size;
    S32 m_memoryBlock;
    vector<Dot> m_dots;

public:
    S32 minX, maxX;       //Feeeeee!.. I know!
    S32 minY, maxY;
    S32 maxZ;

private:
    void _init();

public:
    void push( Dot dot );
    void push( PictureObject spot );
    void calculateMinMaxDotsParams();
    matrix<F32> * CRTmatrixOfThis();

    S32 getSize(){ return m_size; }
    Dot getDot( S32 n ){ return m_dots.at( n ); }
};

#endif // PICTUREOBJECT_H
