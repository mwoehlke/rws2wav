#ifndef _rwtypes_h
#define _rwtypes_h

#include <cstdint>

namespace rws
{
  enum type : uint32_t
  {
    audio_container = 0x80d,
    audio_header = 0x80e,
    audio_data = 0x80f,
    invalid = 0xffffffff
  };

  enum class io_mode
  {
    read,
    write,
  };
}

#endif
