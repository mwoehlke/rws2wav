#include "rwtrack.h"

#include "rwfile.h"

//-----------------------------------------------------------------------------
rws::track::track(uint32_t i) : m_id{i}
{
}

//-----------------------------------------------------------------------------
rws::track::~track()
{
}

//-----------------------------------------------------------------------------
void rws::track::read_location(rws::file& f)
{
  f.skip(0x18);
  f.read_be(m_byte_count);
  f.read_be(m_byte_offset);
}

//-----------------------------------------------------------------------------
void rws::track::read_size(rws::file& f)
{
  f.read_be(m_size);
}

//-----------------------------------------------------------------------------
void rws::track::read_name(rws::file& f)
{
  f.read(m_name);
}

//-----------------------------------------------------------------------------
void rws::track::read_data(
  rws::file& f, off64_t data_start,
  uint32_t cluster_size, uint32_t cluster_used_bytes)
{
  // TODO
}
