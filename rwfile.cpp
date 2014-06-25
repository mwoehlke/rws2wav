#include "rwfile.h"

#include "rwexception.h"

#include <cstring>

#include <byteswap.h>
#include <sys/stat.h>

//-----------------------------------------------------------------------------
void rws::bswap(uint32_t& data)
{
    data = bswap_32(data);
}

//-----------------------------------------------------------------------------
rws::file::file(const std::string& filename)
{
  m_handle = fopen64(filename.c_str(), "rb");
  if (!m_handle)
    raise("unable to open input file '", filename, "'");

  struct stat64 si;
  if (fstat64(fileno(m_handle), &si))
    raise("failed to stat input file '", filename, "'");
  m_size = si.st_size;
}

//-----------------------------------------------------------------------------
rws::file::~file()
{
  if (m_handle)
    fclose(m_handle);
}

//-----------------------------------------------------------------------------
void rws::file::seek(off64_t pos, int mode)
{
  if (fseeko64(m_handle, pos, mode) != 0)
    raise("seek error");
}

//-----------------------------------------------------------------------------
void rws::file::skip(off64_t off)
{
  seek(off, SEEK_CUR);
}

//-----------------------------------------------------------------------------
off64_t rws::file::pos() const
{
  return ftello64(m_handle);
}

//-----------------------------------------------------------------------------
void rws::file::read(uint8_t* buffer, size_t size)
{
  if (fread(buffer, 1, size, m_handle) != size)
    raise("read error");
}

//-----------------------------------------------------------------------------
template <> void rws::file::read(std::string& str)
{
  char buffer[16];
  size_t len = 0;

  str.clear();
  for(;;)
  {
    if (fread(buffer, 1, 16, m_handle) != 16)
      raise("read error");

    auto const end =
      reinterpret_cast<char const*>(memchr(buffer, '\0', sizeof(buffer)));

    if (end)
    {
      str.insert(len, buffer, static_cast<size_t>(end - buffer));
      return;
    }

    str.insert(len, buffer, sizeof(buffer));
    len += sizeof(buffer);
  }
}
