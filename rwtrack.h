#ifndef _rwtrack_h
#define _rwtrack_h

#include <cstdint>

namespace rws
{
  //---------------------------------------------------------------------------
  class track
  {
  public:
    track(uint32_t);
    ~track();

    uint32_t id() { return m_id; }

  protected:
    uint32_t m_id;
  };
}

#endif
