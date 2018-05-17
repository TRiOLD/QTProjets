////////////////////////////////////
// Celestial Mechanics
//
// Author: TRiOLD
// DevEnvir: QT Cr 4.4.1
//
// 13.11.17				upd 00.00.00
////////////////////////////////////
#include "Improvements.h"
#include "Program.h"

////////////////////////////////////

int main( int argc, char* argv[] )
{
	Program prgm( argc, argv );
	prgm.initialize();
	prgm.process();
	prgm.shutdown();

    return 0;
}

////////////////////////////////////
