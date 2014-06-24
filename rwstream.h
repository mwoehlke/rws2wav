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

    inline track_list const tracks() const { return m_tracks; }

  protected:
    chunk m_header;
    chunk m_data;

    uint32_t m_track_count;
    track_list m_tracks;
  };
}

#endif
