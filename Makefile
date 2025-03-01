# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g -I./dep/glad/include -I./include

# Target executable
TARGET = ./bin/main.exe

# Source and object files
SRCS = $(wildcard ./src/*.cpp) ./dep/glad/src/glad.c
OBJS = $(patsubst ./src/%.cpp, ./bin/%.o, $(filter-out ./src/main.cpp, $(wildcard ./src/*.cpp))) ./bin/glad.o

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): ./bin/main.o $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET) -lglfw -ldl

# Rule for object files
./bin/%.o: ./src/%.cpp ./include/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Special Rules
./bin/glad.o: ./dep/glad/src/glad.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

./bin/main.o: ./src/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run target
run: $(TARGET)
	$(TARGET)

# Clean target
clean:
	rm -f $(TARGET) ./bin/*.o

# Credit: Most of this was copied & pasted from ChatGPT.
#		  I made some minor changes.