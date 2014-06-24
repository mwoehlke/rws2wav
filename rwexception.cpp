#include "rwexception.h"

//-----------------------------------------------------------------------------
void rws::raise(const std::string& error)
{
  throw exception(error);
}
