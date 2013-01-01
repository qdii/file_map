#define HAVE_SYS_MMAN_H 1
#include "file_map.h"
#include "qlog.hpp"
#include <UnitTest++/UnitTest++.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace fm;
// -------------------------------------------------------------------------- //
static unsigned test_number = 1;
void startLogging();
// -------------------------------------------------------------------------- //
struct temporary_file
{
    temporary_file()
        :m_name( new char[16] )
    {
        strcpy( m_name, "unittestsXXXXXX" );
        mktemp( m_name );
    }

    const char * name() const
    {
        return m_name;
    }

    ~temporary_file()
    {
        std::remove( m_name );
        delete [] m_name;
    }

private:
    char * m_name;
};

int main()
{
    qlog::init();

    startLogging();
    const int ret = UnitTest::RunAllTests();
    qlog::destroy();

    return ret;
}
// -------------------------------------------------------------------------- //
void startLogging()
{
    using namespace qlog;
    set_output( std::cout );
    set_loglevel( qlog::loglevel::info );

    info.prepend() << "[" << color( gray ) << ".." << color() << "] ";
    warning.prepend() << "[" << color( green ) << "ww" << color() << "] ";
    error.prepend() << "[" << color( red, true ) << "EE" << color() << "] " << color( white, true );
}
// -------------------------------------------------------------------------- //
TEST( CreateFileAndMap )
{
    qlog::info << test_number++ << ". CreateFileAndMap\n";

    // create a file
    temporary_file file;

    // write something into it
    {
        std::ofstream temp_file( file.name() );
        temp_file << "bla";
    }

    // try and map it
    try
    {
        file_map mapping( file.name() );
    }
    catch( ... )
    {
        CHECK( 0 );
    }
}
// -------------------------------------------------------------------------- //
TEST( MapAndRead )
{
    qlog::info << test_number++ << ". MapAndRead\n";
    std::string input( "bla" );
    // create a file
    temporary_file file;

    // write something into it
    {
        std::ofstream temp_file( file.name() );
        temp_file << input;
    }

    // try and map it
    try
    {
        file_map mapping( file.name() );
        CHECK( mapping.size() >= 3 );
        const std::string contents( mapping.begin(), mapping.end() );

        CHECK_EQUAL( input, contents );
    }
    catch( ... )
    {
        CHECK( 0 );
    }
}
// -------------------------------------------------------------------------- //
TEST( MapNoneExistingFile )
{
    qlog::info << test_number++ << ". MapNoneExistingFile\n";
    bool exceptionThrown = false;
    try
    {
        file_map mapping( "somethingrandomthatdoesnotexist" );
    }
    catch ( const invalid_file & file )
    {
        exceptionThrown = true;
    }

    CHECK( exceptionThrown );
}
// -------------------------------------------------------------------------- //
TEST( MapZeroByteFile )
{
    qlog::info << test_number++ << ". MapZeroByteFile\n";
    bool exceptionThrown = false;

    // create a file
    temporary_file file;

    // try and map it
    try
    {
        file_map mapping( file.name() );
    }
    catch( ... )
    {
        exceptionThrown = true;
    }

    CHECK( exceptionThrown );
}
