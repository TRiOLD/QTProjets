#include "Catalog.h"

Catalog::Catalog()
{
    m_minRa = 0.0;
    m_maxRa = 0.0;
    m_minDec = 0.0;
    m_maxDec = 0.0;
    m_minMag = 0.0;
    m_maxMag = 0.0;
}

Catalog::~Catalog()
{

}

void Catalog::init()
{

}

void Catalog::addObjects( Catalog * object )
{
    for( S32 i = 0; i < object->getObjectCount(); i++ )
        addObject( object->getObject( i ) );
}

void Catalog::calculateMinMaxParameters()
{
    if( m_minRa == m_maxRa == m_minDec == m_maxDec == 0.0 )
        return;

    m_minRa = 2 * PI;
    m_maxRa = 0.0;
    m_minDec = PI / 2;
    m_maxDec = -PI / 2;
    m_minMag = 27;
    m_maxMag = -27.0;

    U32 N = getObjectCount();
    for( U32 i = 0; i < N; i++ )
    {
        if( m_objects[i].getRa() == 0 )
            continue;

        if( m_minRa > m_objects[i].getRa() )  m_minRa = m_objects[i].getRa();
        if( m_maxRa < m_objects[i].getRa() )  m_maxRa = m_objects[i].getRa();
        if( m_minDec > m_objects[i].getDec() )  m_minDec = m_objects[i].getDec();
        if( m_maxDec < m_objects[i].getDec() )  m_maxDec = m_objects[i].getDec();
        if( m_minMag > m_objects[i].getMag() )  m_minMag = m_objects[i].getMag();
        if( m_maxMag < m_objects[i].getMag() )  m_maxMag = m_objects[i].getMag();
    }
}

bool Catalog::writeToFile( string fileName )
{
    m_file.open( fileName.c_str(), ios_base::out );
    if ( !m_file.is_open() ) return false;

 // bool isClasters = m_objects[0].getType() == CelestialBodyType_Claster; //По первому обьекту
    bool isClasters = true;

    if( isClasters )
    {
        m_file << "id" << "\t" << "Ra" << "\t" << "Dec" << "\t" << "Prx" << "\t"
                << "Rad" << "\t" << "MuRa" << "\t" << "MuDec" << "\t" << "Quality" << endl;
    }
    else
    {
        m_file << "id" << "\t" << "Ra" << "\t" << "Dec" << "\t" << "Prx" << "\t"
                << "MuRa" << "\t" << "MuDec" << "\t" << "Mag" << endl;
    }



    U64 N = getObjectCount();
    for( U64 i = 0; i < N; i++ )
        m_file << m_objects[i].getAllParameters() << endl;

    m_file.close();
	return true;
}

bool Catalog::readFile( string fileName )
{
    m_file.open( fileName.c_str(), ios_base::in );
    if ( !m_file.is_open() ) return false;

    string header;
    getline( m_file, header );

    U64 ID = 0;
    while( !m_file.eof() )
    {
        CelestialBody body;
        D64 Ra, Dec, Prx, Rad, MuRa, MuDec, Quality;
        Ra = Dec = Prx = Rad = MuRa = MuDec = Quality = 0.0;

        m_file >> ID >> Ra >> Dec >> Rad >> MuRa >> MuDec >> Quality;
        body.init( ID, Ra, Dec, 0.0, Rad, 0.0, 0.0, CelestialBodyType_Claster );

        addObject( body );
        ID++;
    }

    m_file.close();
	return true;
}

bool Catalog::readFile_pmaPart( string fileName )
{
    m_file.open( fileName.c_str(), ios_base::in );
    if ( !m_file.is_open() ) return false;

    U32 fileSize = 0;
    string fileStr;
    while( getline( m_file, fileStr ) )
        fileSize++;

    m_file.close();     //Лучше seek!
    m_file.open( fileName.c_str(), ios_base::in );
    m_objects.resize( fileSize );

    string header;
    getline( m_file, header );

    Processing * PR = new Processing();

    U64 ID = 0;
    while( getline( m_file, fileStr ) )
    {
        CelestialBody body;
        STRS w = PR->divideByWordsSrtings( fileStr, 21 );

        D64 ra = stod( w[0] ), dec = stod( w[1] );

        if( ra >= 2 * PI - 0.0001 )   continue;
        if( dec >= 2 * PI - 0.0001 )  continue;

        body.init( ID, ra, dec, 0.0, stod( w[4] ), stod( w[5] ), stod( w[10] ), CelestialBodyType_Unknown );



        m_objects.at( ID ) = body;
        ID++;
    }

     m_file.close();

 /* string header;
    getline( m_file, header );

    U64 ID = 0;
    ios::sync_with_stdio( false );
    while( !m_file.eof() )
    {
        CelestialBody body;
        D64 RA, DEC, eRA, eDEC, MuRAabs, Mudecabs, MuRAmed, MuDEmed, MuRArel, MuDErel, Gmag,
                                j_m, e_jm, h_m, e_hm, k_m, e_km, dt, pmrag, pmdecg, pts_key;

        m_file >> RA >> DEC >> eRA >> eDEC >> MuRAabs >> Mudecabs >> MuRAmed >> MuDEmed >> MuRArel >> MuDErel >> Gmag
               >> j_m >> e_jm >> h_m >> e_hm >> k_m >> e_km >> dt >> pmrag >> pmdecg >> pts_key;

        body.init( ID, RA, DEC, 0.0, MuRAabs, Mudecabs, Gmag, CelestialBodyType_Unknown );

        m_objects.at( ID ) = body;
        ID++;
    }*/

/*
    string header;
    getline( m_file, header );
    istringstream iss( fileStr );

    U64 ID = 0;
    while( getline( m_file, fileStr ) )
    {
        iss.clear();
        iss.str( fileStr );
        CelestialBody body;
        D64 RA, DEC, eRA, eDEC, MuRAabs, Mudecabs, MuRAmed, MuDEmed, MuRArel, MuDErel, Gmag,
                                j_m, e_jm, h_m, e_hm, k_m, e_km, dt, pmrag, pmdecg, pts_key;

        iss >> RA >> DEC >> eRA >> eDEC >> MuRAabs >> Mudecabs >> MuRAmed >> MuDEmed >> MuRArel >> MuDErel >> Gmag
               >> j_m >> e_jm >> h_m >> e_hm >> k_m >> e_km >> dt >> pmrag >> pmdecg >> pts_key;

        body.init( ID, RA, DEC, 0.0, MuRAabs, Mudecabs, Gmag, CelestialBodyType_Unknown );

        m_objects.at( ID ) = body;        
        ID++;
    }

    m_file.close();
*/

/*
    const C8 * fname = fileName.data();
    FILE * file = fopen( "D:\\.pma_files\\PIX\\pma_1.txt", "r" );
    printf( "%s", fname );

    CelestialBody body;
    D64 RA, DEC, eRA, eDEC, MuRAabs, Mudecabs, MuRAmed, MuDEmed, MuRArel, MuDErel, Gmag,
                            j_m, e_jm, h_m, e_hm, k_m, e_km, dt, pmrag, pmdecg, pts_key;

    C8 * c;
    while( *c != 10 )
        fread( c, 1, 1, file );

    U64 ID = 1;
    while( ID < fileSize )
    {
        fscanf( file, "%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
                &RA, &DEC, &eRA, &eDEC, &MuRAabs, &Mudecabs, &MuRAmed, &MuDEmed, &MuRArel, &MuDErel,
                &Gmag, &j_m, &e_jm, &h_m, &e_hm, &k_m, &e_km, &dt, &pmrag, &pmdecg, &pts_key );

        body.init( ID, RA, DEC, 0.0, MuRAabs, Mudecabs, Gmag, CelestialBodyType_Unknown );

        if( !(RA <= 2 * PI + 0.00001 && RA >= 2 * PI - 0.00001) )
        {
            m_objects.at( ID ) = body;
        }

        ID++;

    }
    fclose( file );

*/
    return true;
}

bool Catalog::readFile_gaiaPart( string fileName )
{
    m_file.open( fileName.c_str(), ios_base::in );
    if ( !m_file.is_open() ) return false;

    U32 fileSize = 0;
    string fileStr;
    while( getline( m_file, fileStr ) )
        fileSize++;

    m_file.close();     //Лучше seek!
    m_file.open( fileName.c_str(), ios_base::in );
    m_objects.resize( fileSize );

    Processing * PR = new Processing();

    U64 ID = 0;
    while( getline( m_file, fileStr ) )
    {
        CelestialBody body;
        STRS w = PR->divideByWordsSrtings( fileStr, 4 );

        D64 ra = stod( w[0] ) * 0.0174532925;
        D64 dec = stod( w[1] ) * 0.0174532925;

        body.init( ID, ra, dec, 0.0, 0.0, 0.0, 0.0, CelestialBodyType_Unknown );

        m_objects.at( ID ) = body;
        ID++;
    }

     m_file.close();
}

 Catalog * Catalog::getSubCatalog( D64 minRa, D64 maxRa, D64 minDec, D64 maxDec )
 {
    Catalog * subCatalog = new Catalog();

    U32 I = 0; //только если отсортировано!!!
    while( minDec > m_objects[I].getDec() )//только если отсортировано!!!
        I++;//только если отсортировано!!!



    for( U32 i = I-1; i < getObjectCount(); i++ ) //только если отсортировано!!!
    {
        if( maxDec < m_objects[i].getDec() )    break;  //только если отсортировано!!!

        if( minRa < m_objects[i].getRa() && maxRa > m_objects[i].getRa() &&
            minDec < m_objects[i].getDec() && maxDec > m_objects[i].getDec() )
            subCatalog->addObject( m_objects[i] );
    }

    subCatalog->calculateMinMaxParameters();
    return subCatalog;
 }
