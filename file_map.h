#ifndef FILE_MAP_H
#define FILE_MAP_H
#include <cstring> //size_t
#ifndef FILEMAP_USE_CPP11
#   define FILEMAP_NO_THROW throw()
#   define FILEMAP_DELETE
#else
#   define FILEMAP_NO_THROW noexcept
#   define FILEMAP_DELETE = delete
#endif

namespace fm
{

/**@struct invalid_file
 * @brief An exception thrown if the file cannot be mapped */
struct invalid_file
{
    explicit
    invalid_file( const char * _filename )
        : m_filename( _filename )
    {
    }

    const char * m_filename;
};

/**@struct map_failed
 * @brief An exception thrown when mapping the file fails */
struct map_failed
{
};

/**@struct file_map
 * @author Victor Lavaud
 * @brief Maps a file to memory */
struct file_map
{
    /**@brief Constructs a null-terminated file mapping
     * @param[in] _filename A path to the file to map.
     * @param[in] _read_only Whether the mapping should be protected against writes (not that this
     *            only works if mmap is used to map the file).
     * @throw invalid_file If the file can’t be opened or if its size is 0 byte, map_failed if mapping
     * the file fails. */
    explicit
    file_map( const char * _filename, bool _read_only = false );

    /**@brief Unset a mapping */
    ~file_map() FILEMAP_NO_THROW;

    /**@brief Returns a pointer to the first byte of the mapping */
    char * data();

    /**@brief Returns a const pointer to the first byte of the mapping */
    const char * data() const;

    /**@brief Returns the size of the mapping in bytes */
    size_t size() const;

public: // support for iterators
    typedef char      *     iterator;
    typedef const char   *  const_iterator;

    iterator        begin();
    const_iterator  begin() const;

    iterator        end();
    const_iterator  end() const;

private:
    size_t m_size;
    char * m_region;

private:
    // undefined member functions
    file_map( const file_map & ) FILEMAP_DELETE;
    file_map & operator=( const file_map & ) FILEMAP_DELETE;
};

// -------------------------------------------------------------------------- //
inline
const char * file_map::data() const
{
    return m_region;
}

// -------------------------------------------------------------------------- //
inline
char * file_map::data()
{
    return m_region;
}

// -------------------------------------------------------------------------- //
inline
file_map::iterator file_map::begin()
{
    return data();
}

// -------------------------------------------------------------------------- //
inline
file_map::const_iterator file_map::begin() const
{
    return data();
}

// -------------------------------------------------------------------------- //
inline
file_map::iterator file_map::end()
{
    return data() + size();
}

// -------------------------------------------------------------------------- //
inline
file_map::const_iterator file_map::end() const
{
    return data() + size();
}

// -------------------------------------------------------------------------- //
inline
size_t file_map::size() const
{
    return m_size;
}

} // namespace file_map
#endif // FILE_MAP_H
