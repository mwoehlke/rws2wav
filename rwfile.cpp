#include "rwfile.h"

#include "rwexception.h"

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

