CXX := clang++
CXXFLAGS := -Wundef -Wextra -Wall -Wpedantic -MP

SRC := src
LIB := lib
OBJ := obj
BIN := bin

PROGRAM := compiler

SOURCES := $(shell find $(SRC) -name "*.cpp")

OBJECTS := $(addsuffix .o, $(basename $(SOURCES)))
OBJECTS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))

HEADERS := $(shell find $(LIB) -name "*.hpp")
HEADERDIRS := $(sort $(dir $(HEADERS)))

INCLUDEFLAGS := $(addprefix -I, $(HEADERDIRS))

TARGET = $(BIN)/$(PROGRAM)

# Util

print:
	echo $(SOURCES)
	echo $(OBJECTS)
	echo $(HEADERDIRS)

# Compilation

all: $(PROGRAM)

$(OBJECTS): $(SOURCES)
	${CXX} ${CXXFLAGS} -c $< -o $@

$(PROGRAM): $(OBJECTS)
	$(CXX) ${CXXFLAGS} $(OBJECTS) -o $(TARGET)

# Debug

debug: CXXFLAGS += -g
debug: all

# Clean up

clean:
	rm -rf $(OBJ)/*/* $(BIN)/*

# Misc

# -include $(DEPENDS)

remake: clean all

.PHONY: clean
