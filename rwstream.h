#ifndef _rwstream_h
#define _rwstream_h

#include "rwchunk.h"
#include "rwtrack.h"

#include <memory>
#include <string>
#include <vector>

namespace rws
{
  using track_ptr = std::shared_ptr<track>;
  using track_list = std::vector<track_ptr>;

  //---------------------------------------------------------------------------
  class stream : public chunk
  {
  public:
    stream(std::string const& filename);
    ~stream();

    inline track_list tracks() const { return m_tracks; }

  protected:
    chunk m_header;
    chunk m_data;

    uint32_t m_track_count;
    track_list m_tracks;

    uint32_t m_cluster_size;
    uint32_t m_cluster_used_bytes;

    uint32_t m_sample_rate;
    uint8_t m_channels;
  };
}

#endif
