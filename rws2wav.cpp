#include <sndfile.h>

#include <cstdio>

#include "rwexception.h"
#include "rwstream.h"

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

    for (auto const& track : is.tracks())
    {
    /* for each track
    auto format = SF_FORMAT_WAV;
    auto info = SF_INFO{frames, rate, channels, format, 0, 0};

    auto const outname = filename + ".wav";
    auto os = sf_open(outname.c_str(), SFM_WRITE, &info);
    */
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
