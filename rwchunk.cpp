#include "rwchunk.h"

#include "rwexception.h"
#include "rwtypes.h"

//-----------------------------------------------------------------------------
rws::chunk::chunk() : m_type{invalid}, m_size{0}, m_start{-1}
{
}

//-----------------------------------------------------------------------------
rws::chunk::~chunk()
{
}

//-----------------------------------------------------------------------------
void rws::chunk::read(rws::file& f)
{
  f.read(m_type);
  f.read(m_size);
  f.skip<uint32_t>();

  m_start = f.pos();

  if (end() > f.size())
  {
    raise("invalid chunk size (size ", m_size, " + offset ", m_start,
          " exceeds file size ", f.size());
  }
}
