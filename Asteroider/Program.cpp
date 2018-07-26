#include "Program.h"

Program::Program()
{

}


Program::Program( S32 argc, C8* argv[] )
{
    for( S32 i = 0; i < argc; i++ )
        args.push_back( string( argv[i] ) );
}


Program::~Program()
{

}


void Program::initialize()
{

}


void Program::shutdown()
{

}
