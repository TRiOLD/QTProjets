#ifndef PROGRAM_H
#define PROGRAM_H

#include "Improvements.h"
#include "TableData.h"
#include "Atlas.h"

class Program
{
public:
    Program();
    Program( S32 argc, C8* argv[] );
    ~Program();

private:
    STRS args;

public:
    void initialize();
    void shutdown();
};

#endif // PROGRAM_H
