#include "rwtrack.h"

#include "rwfile.h"

#include <iostream>

#include <byteswap.h>

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
  // open output file
  auto f = file{filename, io_mode::write};

  // write header
  static auto const bits_per_sample = 16;
  static auto const bytes_per_sample = bits_per_sample / 8;
  f.write("RIFF\0\0\0\0WAVEfmt ", 16); // will fill in size later
  f.write<uint32_t>(16); // format chunk size
  f.write<uint16_t>(1); // PCM format
  f.write<uint16_t>(m_channels);
  f.write<uint32_t>(m_sample_rate);
  f.write<uint32_t>(m_sample_rate * m_channels * bytes_per_sample);
  f.write<uint16_t>(m_channels * bytes_per_sample);
  f.write<uint16_t>(bits_per_sample);
  f.write("data", 4);
  f.write<uint32_t>(m_size);

  // write data
  auto const samples = reinterpret_cast<int16_t const*>(m_samples.get());
  auto const sample_count = m_size / bytes_per_sample;
  for (auto i = decltype(sample_count){0}; i < sample_count; ++i)
    f.write(bswap_16(samples[i]));

  // write adjusted size
  auto const total_size = static_cast<uint32_t>(f.pos());
  f.seek(4);
  f.write<uint32_t>(total_size - 8);

  // report
  std::cout << "wrote " << m_name << " (" << sample_count / m_channels
            << " frames) to '" << filename << "'" << std::endl;
}
