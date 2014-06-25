#ifndef _rwtrack_h
#define _rwtrack_h

#include <memory>
#include <string>

#include <cstdint>

namespace rws
{
  class file;

  //---------------------------------------------------------------------------
  class track
  {
  public:
    track(uint32_t);
    ~track();

    inline uint32_t id() { return m_id; }
    inline std::string name() { return m_name; }

    void read_location(file&);
    void read_size(file&);
    void read_name(file&);
    void read_data(file&, off64_t data_start,
                   uint32_t cluster_size, uint32_t cluster_used_bytes);

  protected:
    uint32_t const m_id;

    uint32_t m_byte_count; // bytes used by all clusters, including padding
    uint32_t m_byte_offset;
    uint32_t m_size; // bytes of actual sample data (m_data size)

    std::string m_name;

    std::unique_ptr<unsigned char[]> m_samples;
  };
}

#endif
