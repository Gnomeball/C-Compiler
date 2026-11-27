CXX = clang++
CXXFLAGS = -Wundef -Wextra -Wall -Wpedantic -MP

SRC = src
OBJ = obj
BIN = bin

PROGRAM = compiler

SOURCES = $(basename $(notdir $(shell find $(SRC) -name *.cpp)))
OBJECTS = $(addprefix $(OBJ)/, $(addsuffix .o, $(SOURCES)))
# DEPENDS := $(OBJECTS:.o=.d)

TARGET = $(BIN)/$(PROGRAM)

# Build the compiler by default, creating the output directories first.

all: directories $(PROGRAM)

# Compile each matching src/*.cpp into obj/*.o. Header dependencies are not
# included, so changes to headers alone do not trigger recompilation.
$(OBJ)/%.o: $(SRC)/%.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

# Link the object files into bin/compiler.
$(PROGRAM): $(OBJECTS)
	$(CXX) ${CXXFLAGS} $(OBJECTS) -o $(TARGET)

# Build with debug symbols. Existing objects are not rebuilt just because the
# flags changed; run `make clean` first for a fully debug-enabled build.

debug: CXXFLAGS += -g
debug: all

# Set up; creates any necessary directories.

directories:
	mkdir -p $(OBJ) $(BIN)

# Clean up; removes build directories and their contents.

clean:
	rm -rf $(OBJ) $(BIN)

# Documentation; runs Doxygen using the project's Doxyfile.

docs:
	doxygen

# -include $(DEPENDS)

# Remove build output and build again. With parallel make, clean and all may run
# concurrently; use sequential make for this target.
remake: clean all

.PHONY: clean
