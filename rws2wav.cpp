#include "rwexception.h"
#include "rwstream.h"

#include <cstdio>

//-----------------------------------------------------------------------------
void usage()
{
}

//-----------------------------------------------------------------------------
void convert(std::string const filename)
{
  try
  {
    auto is = rws::stream{filename};
    auto base_name = filename;

    auto ep = base_name.rfind('.');
    base_name = base_name.substr(0, ep);

    for (auto const& t : is.tracks())
    {
      // get name for output file
      std::ostringstream ss;
      ss << base_name << "-tr" << t->id() << ".wav";

      // write track
      try
      {
        t->write(ss.str());
      }
      catch (rws::exception const& e)
      {
        fprintf(stderr, "%s\n", e.what());
      }
    }
  }
  catch (rws::exception const& e)
  {
    fprintf(stderr, "%s\n", e.what());
  }
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{
  if (argc < 1)
  {
    usage();
    return 1;
  }

  // Iterate over input files
  for (int i = 1; i < argc; ++i)
    convert(argv[i]);

  return 0;
}
