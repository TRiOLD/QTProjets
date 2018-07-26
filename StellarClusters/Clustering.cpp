#include "Clustering.h"


Catalog * Clustering::CRTcatalog_TrueGC( Catalog * catPart, CelestialBody * gc )
{
    Catalog * potCat = new Catalog();
    Catalog * resCat = new Catalog();

    D64 GCRa = gc->getParam( 7 );
    D64 GCDec = gc->getParam( 8 );
    D64 GCRad = (gc->getParam( 9 ) + gc->getParam( 10 )) / 4 / 10;

    for( S32 i = 0; i < catPart->getObjCount(); i++ )
    {
        D64 ra = catPart->getObj( i )->getParam( RA );
        D64 dec = catPart->getObj( i )->getParam( DEC );
        D64 rad = sqrt( pow( ra - GCRa, 2 ) + pow( dec - GCDec, 2 ) );

        if( rad <= GCRad )
            potCat->pushBack( catPart->getObj( i ) );
    }

    S32 N = potCat->getObjCount();
    vector<D64> muRas( N ), muDecs( N ), plxs( N );
    S32 itPr = 0, itPx = 0;
    for( S32 i = 0; i < N ; i++ )
    {
        D64 muRa = catPart->getObj( i )->getParam( muRA );
        D64 muDec = catPart->getObj( i )->getParam( muDEC );
        D64 plx = catPart->getObj( i )->getParam( PLX );
        if( muRa && muDec )
        {
            muRas[itPr] = muRa;
            muDecs[itPr] = muDec;
            itPr++;
        }
        if( plx )
        {
            plxs[itPx] = plx;
            itPx++;
        }
    }
    D64 averMuRa = Algorithms::average( muRas, itPr );
    D64 averMuDec = Algorithms::average( muDecs, itPr );
    D64 averPlx = Algorithms::average( plxs, itPx );
    D64 sigmaMuRa = Algorithms::stdDeviation( muRas, averMuRa, itPr );
    D64 sigmaMuDec = Algorithms::stdDeviation( muDecs, averMuDec, itPr );
    D64 sigmaPlx = Algorithms::stdDeviation( plxs, averPlx, itPx );

    const D64 epsilon = 0.00000001;
    D64 sigmaPrprMtn = 0.0, sigmaPrprMtn_temp = 0.0;
    do
    {
        sigmaPrprMtn_temp = sigmaPrprMtn;
        sigmaPrprMtn = sqrt( pow( sigmaMuRa, 2 ) + pow( sigmaMuDec, 2 ) );

        for( S32 i = 0; i < itPr ; i++ )
        {
            D64 sMuRa = averMuRa - muRas[i];
            D64 sMuDec = averMuDec - muDecs[i];
            D64 s = sqrt( pow( sMuRa, 2 ) + pow( sMuDec, 2 ) );

            if( s > 3 * sigmaPrprMtn )
            {
                muRas[i] = averMuRa;
                muDecs[i] = averMuDec;
            }
        }
        averMuRa = Algorithms::average( muRas, itPr );
        averMuDec = Algorithms::average( muDecs, itPr );
        sigmaMuRa = Algorithms::stdDeviation( muRas, averMuRa, itPr );
        sigmaMuDec = Algorithms::stdDeviation( muDecs, averMuDec, itPr );

    }while( sigmaPrprMtn - sigmaPrprMtn_temp > epsilon );

    D64 sigmaPlx_temp = 0.0;
    do
    {
        sigmaPlx_temp = sigmaPlx;
        for( S32 i = 0; i < itPx ; i++ )
            if( plxs[i] > 3 * sigmaPlx )
                plxs[i] = averPlx;

         averPlx = Algorithms::average( plxs, itPx );
         sigmaPlx = Algorithms::stdDeviation( plxs, averPlx, itPx );

    }while( sigmaPlx - sigmaPlx_temp > epsilon );

    D64 GCMuRa = averMuRa;
    D64 GCMuDec = averMuDec;
    D64 GCPlx = averPlx;
    D64 GCMu = sqrt( pow( GCMuRa, 2 ) + pow( GCMuDec, 2 ) );

    for( S32 i = 0; i < catPart->getObjCount(); i++ )
    {
        D64 ra = catPart->getObj( i )->getParam( RA );
        D64 dec = catPart->getObj( i )->getParam( DEC );
        D64 rad = sqrt( pow( ra - GCRad, 2 ) + pow( dec - GCRad, 2 ) );
        D64 muRa = catPart->getObj( i )->getParam( muRA );
        D64 muDec = catPart->getObj( i )->getParam( muDEC );
        D64 mu = sqrt( pow( muRa, 2 ) + pow( muDec, 2 ) );
        D64 plx = catPart->getObj( i )->getParam( PLX );

        if( mu < GCMu + 3 * sigmaPrprMtn && mu > GCMu - 3 * sigmaPrprMtn &&
                plx < GCPlx + 3 * sigmaPlx && plx > GCPlx - 3 * sigmaPlx &&
                    rad <= GCRad )
            resCat->pushBack( catPart->getObj( i ) );
    }

    delete potCat;
    return resCat;
}
