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

    void set_layout(uint8_t channels, uint32_t sample_rate);

    void write(std::string const& filename);

  protected:
    uint32_t const m_id;

    uint32_t m_byte_count; // bytes used by all clusters, including padding
    uint32_t m_byte_offset;
    uint32_t m_size; // bytes of actual sample data (m_data size)

    std::string m_name;

    uint8_t m_channels;
    uint32_t m_sample_rate;
    std::unique_ptr<uint8_t[]> m_samples;
  };
}

#endif
