////////////////////////////////////
//class matrix<TYPE>
//
//C++98/03 (noLib)
//
//
//byTRiOLD -l-      04.18 upd: 05.18
////////////////////////////////////
#pragma once

////////////////////////////////////
template <typename TYPE>
class matrix
{
private:
    TYPE ** m_DATA;
    char ** m_conditionDATA;

    TYPE * m_falseDATA;
    char * m_falseCondDATA;

    bool m_isEmpty;
    bool m_hasCond;
    int m_colmsX;
    int m_rowsY;

    ///////////////
public:
    matrix()
    {
        _setup();
        _init();
    }
    matrix( const int rowsY, const int colmsX )
    {
        _setup();
        create( rowsY, colmsX, false );
    }
    matrix( const int rowsY, const int colmsX, const bool hasCond )
    {
        _setup();
        create( rowsY, colmsX, hasCond );
    }
    matrix( const matrix &M )
    {
        _setup();
        create( M );
    }
    virtual ~matrix()
    {
        clear();
        _deleteFalseData();
    }

    ///////////////
private:
    void _setup()
    {
        m_DATA = NULL;
        m_conditionDATA = NULL;

        m_falseDATA = NULL;
        m_falseCondDATA = NULL;
        _createFalseData();
    }

    void _init()
    {
        m_isEmpty = true;
        m_hasCond = false;
        m_colmsX = 0;
        m_rowsY = 0;
    }

    void _createDATA()
    {
        TYPE ** DATA = new TYPE *[m_rowsY];
        for( int j = 0; j < m_rowsY; j++ )
            DATA[j] = new TYPE [m_colmsX];

        m_DATA = DATA;
        m_isEmpty = false;
    }

    void _createStatusOfData()
    {
        char ** conditionDATA  = new char *[m_rowsY];
        for( int j = 0; j < m_rowsY; j++ )
            conditionDATA[j] = new char [m_colmsX];
        m_conditionDATA = conditionDATA;

        resetCond();
    }

    void _createFalseData()
    {
        m_falseDATA = new TYPE;
        m_falseCondDATA = new char;
    }

    void _resetFalseData()
    {
        *m_falseDATA = 42;
        *m_falseCondDATA = (char)42;
    }

    void _deleteFalseData()
    {
        delete m_falseDATA;
        delete m_falseCondDATA;
    }

    ///////////////
public:
    TYPE * operator [] ( const int j )
    {
        return m_DATA[j];
    }

    matrix operator = ( const matrix &M2 )
    {
        this->create( M2 );
        matrix<TYPE> res( M2 );
        return res;
    }

    void create( const int rowsY, const int colmsX, const bool hasCond )
    {
        if( !isEmpty() )	clear();

        _init();

        if( colmsX > 0 && rowsY > 0 )
        {
            m_colmsX = colmsX;
            m_rowsY = rowsY;
            _createDATA();
            initDATA( 0 );
        }
        if( hasCond )
            createCond();
    }

    void create( const matrix &M )
    {
        int X = M.getSizeX();
        int Y = M.getSizeY();
        bool CN = M.isHasCond();
        create( Y, X, CN );
        for( int j = 0; j < Y; j++ )
            for( int i = 0; i < X; i++ )
                at( j, i ) = M.at( j, i );
        if( CN )
            for( int j = 0; j < Y; j++ )
                for( int i = 0; i < X; i++ )
                    atStatus( j, i ) = M.atStatus( j, i );
    }

    bool initDATA( TYPE value )
    {
        if( m_isEmpty )
            return false;

        for( int j = 0; j < m_rowsY; j++ )
            for( int i = 0; i < m_colmsX; i++ )
                m_DATA[j][i] = value;
        return true;
    }

    bool createCond()
    {
        if( !m_isEmpty )
        {
            m_hasCond = true;
            _createStatusOfData();
            return true;
        }
        return false;
    }

    void clear()
    {
        if( m_DATA )
        {
            for( int j = 0; j < m_rowsY; j++ )
                delete[] m_DATA[j];
            delete [] m_DATA;
        }

        if( m_conditionDATA )
        {
            for( int j = 0; j < m_rowsY; j++ )
                delete[] m_conditionDATA[j];
            delete [] m_conditionDATA;
        }

        _init();
    }

    bool resetCond()
    {
        if( m_isEmpty && !m_hasCond )
            return false;

        for( int j = 0; j < m_rowsY; j++ )
            for( int i = 0; i < m_colmsX; i++ )
                m_conditionDATA[j][i] = (char)0;
        return true;
    }

    TYPE &at( const int j, const int i )
    {
        if( j >= 0 && j < m_rowsY && i >= 0 && i < m_colmsX )
            return m_DATA[j][i];
        else
        {
            _resetFalseData();
            return (*m_falseDATA);
        }
    }

    char &atStatus( const int j, const int i )
    {
        if( j >= 0 && j < m_rowsY && i >= 0 && i < m_colmsX && m_hasCond )
            return m_conditionDATA[j][i];
        else
        {
            _resetFalseData();
            return (*m_falseCondDATA);
        }
    }

    TYPE at( const int j, const int i ) const { return m_DATA[j][i]; }
    char atStatus( const int j, const int i ) const { return m_conditionDATA[j][i]; }

    int getSizeX() const { return m_colmsX; }
    int getSizeY() const { return m_rowsY; }
    bool isEmpty() const { return m_isEmpty; }
    bool isHasCond() const { return m_hasCond; }
};
