#ifndef _rwchunk_h
#define _rwchunk_h

#include "rwfile.h"

namespace rws
{
  //---------------------------------------------------------------------------
  class chunk
  {
  public:
    chunk();
    ~chunk();

    void read(file&);

    inline uint32_t type() const { return m_type; }
    inline uint32_t size() const { return m_size; }

    inline off64_t start() const { return m_start; }
    inline off64_t end() const { return m_start + m_size; }

  protected:
    uint32_t m_type;
    uint32_t m_size;

    off64_t m_start;
  };
}

#endif
