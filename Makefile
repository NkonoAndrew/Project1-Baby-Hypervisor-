CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Source files for the main application
VMM_SRCS = myvmm.cpp
VMM_OBJS = $(VMM_SRCS:.cpp=.o)
VMM_EXEC = myvmm

# Default target
all: $(VMM_EXEC)

# Rule to link the main application
$(VMM_EXEC): $(VMM_OBJS)
	$(CXX) $(CXXFLAGS) -o $(VMM_EXEC) $(VMM_OBJS)

# Generic rule to compile .cpp to .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(VMM_OBJS) $(VMM_EXEC)

.PHONY: all clean
