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
     * @param[in] _read_only Whether the mapping should be protected against writes
     * @throw invalid_file If the file can’t be opened, map_failed if mapping
     * the file fails. */
    explicit
    file_map( const char * _filename, bool _read_only = false );

    /**@brief Unset a mapping */
    ~file_map() FILEMAP_NO_THROW;

    /**@brief Returns a pointer to the first byte of the mapping */
    char * data()
    {
        return m_region;
    }

    /**@brief Returns a const pointer to the first byte of the mapping */
    const char * data() const
    {
        return m_region;
    }

    /**@brief Returns the size of the mapping in bytes */
    size_t size() const
    {
        return m_size;
    }

public: // support for iterators
    typedef char      *     iterator;
    typedef const char   *  const_iterator;

    iterator        begin()
    {
        return data();
    }
    const_iterator  begin() const
    {
        return data();
    }

    iterator        end()
    {
        return data() + size();
    }
    const_iterator  end() const
    {
        return data() + size();
    }

private:
    size_t m_size;
    char * m_region;

private:
    // undefined member functions
    file_map( const file_map & ) FILEMAP_DELETE;
    file_map & operator=( const file_map & ) FILEMAP_DELETE;
};

#endif // FILE_MAP_H
