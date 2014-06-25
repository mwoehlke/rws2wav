#include "rwstream.h"

#include "rwexception.h"
#include "rwtrack.h"
#include "rwtypes.h"

//-----------------------------------------------------------------------------
rws::stream::stream(const std::string& filename)
{
  auto f = file(filename);
  fprintf(stdout, "reading '%s'\n", filename.c_str());

  // read container header
  read(f);

  // search for audio header
  for (;;)
  {
    m_header.read(f);
    if (m_header.type() == audio_header)
      break;
    f.seek(m_header.end());
  }

  // read start of audio header
  uint32_t header_size;
  f.read(header_size);
  f.skip(0x1c);
  f.read_be(m_track_count);
  f.skip(0x2c);

  std::string stream_name;
  f.read(stream_name);

  // read per-track headers
  m_tracks.reserve(m_track_count);
  for (uint32_t i = 0; i < m_track_count; ++i)
  {
    auto track = new rws::track{i};
    track->read_location(f);
    m_tracks.emplace_back(track);
  }
  for (auto t : m_tracks)
    t->read_size(f);
  f.skip(0x10 * m_track_count);
  for (auto t : m_tracks)
    t->read_name(f);

  // read end of audio header
  f.skip(0x10);
  f.read_be(m_cluster_size);
  f.skip(0xc);
  f.read_be(m_cluster_used_bytes);
  f.skip<uint32_t>();
  f.read_be(m_sample_rate);
  f.skip(0x9);
  f.read_be(m_channels);

  // set track layouts
  for (auto t : m_tracks)
    t->set_layout(m_channels, m_sample_rate);

  // search for data header
  f.seek(m_header.end());
  for (;;)
  {
    m_data.read(f);
    if (m_data.type() == audio_data)
      break;
    f.seek(m_data.end());
  }

  // read track data
  auto const start = m_data.start();
  for (auto t : m_tracks)
    t->read_data(f, start, m_cluster_size, m_cluster_used_bytes);
}

//-----------------------------------------------------------------------------
rws::stream::~stream()
{
}
