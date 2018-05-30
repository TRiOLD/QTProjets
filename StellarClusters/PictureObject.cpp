#include "PictureObject.h"

PictureObject::PictureObject()
{
    m_memoryBlock = 1000;
    m_size = 0;
    m_dots.resize( m_memoryBlock );
}


void PictureObject::push( PictureObject::Dot dot )
{
    if( S32(m_dots.size()) < m_size + 1 )
        m_dots.resize( m_dots.size() + m_memoryBlock );

    S32 i = m_size;
    m_dots.at( i ) = dot;
    m_size++;
}


void PictureObject::push( PictureObject spot )
{
    if( S32(m_dots.size()) < m_size + spot.getSize() )
    {
        S32 multi = (m_size + spot.getSize()) / m_dots.size();
        m_dots.resize( m_dots.size() + multi * m_memoryBlock );
    }

    S32 i = m_size;
    for(S32 j = 0; j < spot.getSize(); i++, j++ )
        m_dots.at( i ) = spot.getDot( j );

    m_size += spot.getSize();
}


void PictureObject::calculateMinMaxDotsParams()
{
    if( m_dots.empty() )    return;

    minX = 65536; maxX = 0;
    minY = 65536; maxY = 0;
    maxZ = 0.0;
    for( S32 i = 0; i < m_size; i++ )
    {
        if( m_dots[i].X > maxX )    maxX = m_dots[i].X;
        if( m_dots[i].X < minX )    minX = m_dots[i].X;
        if( m_dots[i].Y > maxY )    maxY = m_dots[i].Y;
        if( m_dots[i].Y < minY )    minY = m_dots[i].Y;
        if( m_dots[i].Z > maxZ )    maxZ = m_dots[i].Z;
    }
}


matrix<F32> * PictureObject::CRTmatrixOfThis()
{

}
