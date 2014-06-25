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
  // reserve storage
  m_samples.reset(new uint8_t[m_size]);
  auto buffer = m_samples.get();

  // seek to start of track data
  f.seek(data_start + m_byte_offset);

  // read track data
  uint32_t remaining = m_size;
  while (remaining)
  {
    // determine amount to read for this cluster
    auto const cluster_start = f.pos();
    auto const to_read = std::min(remaining, cluster_used_bytes);

    // read cluster
    f.read(buffer, to_read);
    remaining -= to_read;
    buffer += to_read;

    // seek to next cluster
    f.seek(cluster_start + cluster_size);
  }
}
