#include "Improvements.h"
#include "Program.h"

int main( int argc, char* argv[] )
{
    Program prgm( argc, argv );
    prgm.initialize();
    prgm.process2();
    prgm.shutdown();

    return 0;
}
