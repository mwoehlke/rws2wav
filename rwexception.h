#ifndef _rwexception_h
#define _rwexception_h

#include <exception>
#include <sstream>
#include <string>

namespace rws
{
  template <typename... Args> void raise(Args...);
  extern void raise(std::string const& error);

  //---------------------------------------------------------------------------
  struct expand
  {
    template <typename... Args> expand(Args&...) {}
  };

  //---------------------------------------------------------------------------
  class exception : public std::exception
  {
  public:
    exception(std::string const& w) : m_what(w) {}
    ~exception() throw() {}

    inline char const* what() const throw() override
    { return m_what.c_str(); }

  protected:
    std::string const m_what;
  };
}

//-----------------------------------------------------------------------------
template <typename... Args>
void rws::raise(Args... args)
{
  std::ostringstream ss;
  expand{(ss << args)...};
  raise(ss.str());
}

#endif
