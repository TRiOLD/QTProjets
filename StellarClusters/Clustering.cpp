#include "Clustering.h"

Catalog * Clustering::CRTcatalog_TrueGC( CatalogGAIA2 * catPart, CelestialBody * gc )
{
    Catalog * resCat = new Catalog();
    Catalog * allCat = new Catalog();

    D64 GCRa = gc->getParam( 7 );
    D64 GCDec = gc->getParam( 8 );
    D64 GCRad = gc->getParam( 9 ) + gc->getParam( 10 );    // Нелогично, но так надо!

    for( S32 i = 0; i < catPart->getObjCount(); i++ )
    {
        D64 ra = catPart->getObj( i )->getParam( RA );
        D64 dec = catPart->getObj( i )->getParam( DEC );
        if( pow( GCRa - ra, 2 ) + pow( GCDec - dec, 2 ) <= 10 * GCRad )
            allCat->pushBack( catPart->getObj( i ) );
    }



    delete allCat;
    return resCat;
}

