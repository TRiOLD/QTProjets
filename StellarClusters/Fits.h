////////////////////////////////////
#pragma once

#include "Improvements.h"
#include "Processing.h"

////////////////////////////////////
class Processing;

////////////////////////////////////
enum FitsKeyWord
{
    SIMPLE, BITPIX, NAXIS, NAXIS1, NAXIS2, END,
    FitsKeyWord_MAX
};

class Fits
{
public:
    Fits();
    ~Fits();

private:
    fstream m_file;
    string m_fileName;
    S32 m_metadataSize;

    string m_headerKeyWords[FitsKeyWord_MAX];
    S32 m_headerDATA[FitsKeyWord_MAX];
    MATRIX<F32> * m_DATA;

private:
    bool scanfHeaderFromFile();
    bool printfHeaderToFile();
    bool readDATAFromFile();
    bool writeDATAToFile();

public:
    void readFitsFile();
    void writeFitsFile();

    void setFile( string fileName ){ m_fileName = fileName; }
    MATRIX<F32> * getDATA(){ return m_DATA; }
    void setDATA( MATRIX<F32> * DATA )
    {
        m_DATA = DATA;
        m_headerDATA[NAXIS1] = DATA->m_x;
        m_headerDATA[NAXIS2] = DATA->m_y;
    }

private:
    Processing * PRC;
};

////////////////////////////////////
