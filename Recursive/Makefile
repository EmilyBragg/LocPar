CXX = g++ --std=c++11
CC  = $(GCC)
GCC = g++ --std=c++11
LD  = $(CXX)

LIBS =

WFLAGS = -Wall

OPTFLAGS = #-0

CFLAGS = $(OPTFLAGS) $(WFLAGS)

CXXSRCS = $(shell ls *.cpp)

OBJS = $(CXXSRCS:.cpp=.o)

PROGRAM = matmul

all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(LD) -o $@ $(OBJS) $(LIBS)
