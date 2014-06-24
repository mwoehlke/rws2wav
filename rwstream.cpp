#include "rwstream.h"

#include "rwexception.h"
#include "rwtypes.h"

//-----------------------------------------------------------------------------
rws::stream::stream(const std::string& filename)
{
  auto f = file{filename};
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

  // read audio header
  uint32_t header_size;
  f.read(header_size);
  f.skip(0x1c);
  printf("tracks at %llx\n", f.pos());
  f.read_be(m_track_count);
  f.skip(0x2c);

  // TODO remaining headers
  printf("track count: %u\n", m_track_count);

  // search for data header
  f.seek(m_header.end());
  for (;;)
  {
    m_data.read(f);
    if (m_data.type() == audio_data)
      break;
    f.seek(m_data.end());
  }
}

//-----------------------------------------------------------------------------
rws::stream::~stream()
{
}
