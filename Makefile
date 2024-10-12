# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -g  -std=c++11

# Define the source files
MAIN = main.cpp

# Use wildcard to find all .cpp files in src/ and its subdirectories
SRC = $(wildcard src/**/*.cpp)

# Combine main.cpp and the source files
ALL_SRC = $(MAIN) $(SRC)

# Convert all .cpp files to .o files
OBJ = $(ALL_SRC:.cpp=.o)

# Target executable
TARGET = myprogram

# Rule to link the object files
$(TARGET): $(OBJ)
	$(CXX) -o $@ $^

# Rule to compile each .cpp file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object and executable files
clean:
	rm -f $(OBJ) $(TARGET)
