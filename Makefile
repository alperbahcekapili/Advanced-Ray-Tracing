# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -ansi -O3 -g  -std=c++11   
# CXXFLAGS = -Wall -Wextra -pedantic -ansi   -std=c++11 
# Define the source files
MAIN = main.cpp

# Use wildcard to find all .cpp files in src/ and its subdirectories
SRC = $(wildcard src/**/*.cpp)

# Combine main.cpp and the source files
ALL_SRC = $(MAIN) $(SRC)

# Convert all .cpp files to .o files
OBJ = $(ALL_SRC:.cpp=.o)

# Target executable
TARGET = raytracer

# Rule to link the object files into the final executable
$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ `pkg-config --libs opencv4`

# Rule to compile each .cpp file into an object file
# Ensure correct folder structure for .o files by creating the directory
%.o: %.cpp
	@mkdir -p $(dir $@)  # Create directories as needed
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

# Clean up object and executable files
clean:
	rm -f $(OBJ) $(TARGET)
