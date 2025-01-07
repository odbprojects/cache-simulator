# Makefile

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -g -Wall -Wextra -pedantic

# Source files and object files
COMMON_CPP_SRCS = main.cpp cache.cpp
COMMON_CPP_OBJS = $(COMMON_CPP_SRCS:.cpp=.o)

# Executable name
EXE = csim

# Default target
all: $(EXE)

# Rule for building object files from C++ source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule for building the executable
$(EXE): $(COMMON_CPP_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(COMMON_CPP_OBJS)

# Clean up build artifacts
clean:
	rm -f *.o $(EXE)
