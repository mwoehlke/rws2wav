#include "rwtrack.h"

#include "rwfile.h"

#include <sndfile.h>

#include <iostream>

//-----------------------------------------------------------------------------
rws::track::track(uint32_t i)
  : m_id{i}, m_byte_count{0}, m_byte_offset{0}, m_size{0},
    m_channels{0}, m_sample_rate{0}
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

//-----------------------------------------------------------------------------
void rws::track::set_layout(uint8_t channels, uint32_t sample_rate)
{
  m_channels = channels;
  m_sample_rate = sample_rate;
}

//-----------------------------------------------------------------------------
void rws::track::write(const std::string& filename)
{
  // set up format information
  auto frames = sf_count_t(m_size / (sizeof(uint16_t) * m_channels));
  auto sample_rate = int(m_sample_rate);
  auto format = SF_FORMAT_WAV | SF_FORMAT_PCM_16 | SF_ENDIAN_BIG;
  auto info = SF_INFO{frames, sample_rate, m_channels, format, 0, 0};

  // open output file
  auto os = sf_open(filename.c_str(), SFM_WRITE, &info);

  // write data
  sf_write_raw(os, m_samples.get(), m_size);

  // close output file
  sf_close(os);

  // report
  std::cout << "wrote " << m_name << " (" << frames << " frames)"
            << " to '" << filename << "'" << std::endl;
}
