#ifndef _rwexception_h
#define _rwexception_h

#include <exception>
#include <sstream>
#include <string>

namespace rws
{
  inline void format(std::ostream&) {}
  extern void raise(std::string const& error);
  template <typename... Args> void raise(Args...);

  //---------------------------------------------------------------------------
  template <typename T, typename... Args>
  void format(std::ostream& os, T const& first_arg, Args const&... args)
  {
    os << first_arg;
    format(os, args...);
  }

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
  format(ss, args...);
  raise(ss.str());
}

#endif
