#ifndef _rwfile_h
#define _rwfile_h

#include <string>

#include <cstdint>
#include <cstdio>

#include <byteswap.h>

#include "rwexception.h"

namespace rws
{
  extern void bswap(uint32_t&);

  //---------------------------------------------------------------------------
  class file
  {
  public:
    file(const std::string& filename);
    ~file();

    template <typename T> void read(T& data);
    template <typename T> void read_be(T& data);

    void seek(off64_t pos, int mode = SEEK_SET);
    void skip(off64_t off);

    template <typename T> void skip();

    inline off64_t size() const { return m_size; }
    off64_t pos() const;

  protected:
    FILE* m_handle;
    off64_t m_size;
  };
}

//-----------------------------------------------------------------------------
template <typename T>
void rws::file::read(T& data)
{
  if (fread(&data, sizeof(T), 1, m_handle) != 1)
    raise("read error");
}

//-----------------------------------------------------------------------------
template <typename T> void rws::file::read_be(T& data)
{
  read(data);
  bswap(data);
}

//-----------------------------------------------------------------------------
template <typename T> void rws::file::skip()
{
  skip(sizeof(T));
}

#endif
