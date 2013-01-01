#include "file_map.h"

#if HAVE_SYS_MMAN_H == 1
#   include <sys/mman.h>
#   include <sys/stat.h>
#   include <sys/types.h>
#   include <unistd.h>
#   include <fcntl.h>
#else
#   include <fstream>
#endif

namespace fm
{
#if HAVE_SYS_MMAN_H == 1
file_map::file_map( const char * _filename, bool _read_only )
    :m_size( 0 )
    ,m_region( 0 )
{
    const int file_descriptor
        = open( _filename, _read_only ? O_RDONLY : O_RDWR );

    if( -1 != file_descriptor )
    {
        // getting the file size
        struct stat st;
        const int ret = fstat( file_descriptor, &st );

        if( ret == -1 )
            throw invalid_file( _filename );

        m_size = st.st_size;

        // map the region
        m_region =
            static_cast<char *>(
                mmap(
                    0, m_size,
                    _read_only ? PROT_READ : PROT_READ | PROT_WRITE,
                    MAP_PRIVATE, file_descriptor, 0
                )
            );

        if( m_region == reinterpret_cast<void *>( -1 ) )
            throw map_failed();
    }
    else
        throw invalid_file( _filename );
}

// -------------------------------------------------------------------------- //

file_map::~file_map() FILEMAP_NO_THROW
{
    // unmap the region if it was mapped
    if( m_region )
        munmap( m_region, m_size );
}

// -------------------------------------------------------------------------- //

#else // HAVE_SYS_MMAN_H

file_map::file_map( const char * _filename, bool /* _read_only */ )
    :m_size( 0 )
    ,m_region( 0 )
{
    // get the file size
    std::ifstream filestream(
        _filename,
        std::ios_base::binary | std::ios_base::in | std::ios_base::ate
    );

    if( !filestream.is_open() )
        throw invalid_file( _filename );

    filestream.exceptions( std::ifstream::failbit | std::ifstream::badbit );
    const std::streamoff filesize = filestream.tellg();
    char * region = 0;

    // copy the file to memory
    try
    {
        region = new char[filesize + 1];
    }
    catch( const std::bad_alloc & )
    {
        throw map_failed();
    }

    try
    {
        filestream.seekg( std::ios_base::beg );
        filestream.read( region, filesize );
        filestream.close();
    }
    catch( const std::ifstream::failure & )
    {
        delete [] region;
        throw invalid_file( _filename );
    }

    region[filesize] = '\0';

    // everything seems fine, lets build the object
    m_region = region;
    m_size = filesize;
}

// -------------------------------------------------------------------------- //

file_map::~file_map() FILEMAP_NO_THROW
{
    delete [] m_region;
}
#endif // HAVE_SYS_MMAN_H

} // file_map
