# Makefile

CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic

# C++ source files that are used in all versions of the executable
COMMON_CPP_SRCS = main.cpp cache.cpp
COMMON_CPP_OBJS = $(COMMON_CPP_SRCS:.cpp=.o)

EXE = csim

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o : %.S
	$(CC) -c $< -o $@

$(EXE) : $(COMMON_CPP_OBJS)
	$(CXX) -o $@ $(COMMON_CPP_OBJS)

clean :
	rm -f *.o $(EXE)