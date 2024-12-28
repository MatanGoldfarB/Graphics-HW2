# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -g

# Target executable
TARGET = raytracer

# Source files
SRCS = HW2.cpp Scene.cpp Intersection.cpp Object.cpp Ligth.cpp Vector.cpp Ray.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)
