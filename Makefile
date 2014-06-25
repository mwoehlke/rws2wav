CXXFLAGS = \
  -Wall \
  -Wextra \
  -Werror=return-type

LDFLAGS =

OBJECTS = \
  rwchunk.o \
  rwexception.o \
  rwfile.o \
  rwstream.o \
  rwtrack.o \
  rws2wav.o

all: rws2wav

rws2wav: $(OBJECTS)
	$(CXX) -std=c++11 $(LDFLAGS) -lsndfile -o $@ $^

%.o: %.cpp
	$(CXX) -std=c++11 $(CXXFLAGS) -c -o $@ $^
