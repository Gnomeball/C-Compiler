CC = clang++
CCFLAGS = -Wextra -Wall -Wpedantic -MMD -MP

src_dir = src
obj_dir = obj
bin_dir = bin

sources = $(basename $(notdir $(shell find $(src_dir) -name *.cpp)))
objects = $(addprefix $(obj_dir)/, $(addsuffix .o, $(sources)))
depends := $(objects:.o=.d)

target = $(bin_dir)/compiler

# Compilation

all: directories compiler

obj/%.o: src/%.cpp
	${CC} ${CCFLAGS} -c $< -o $@

compiler: $(objects)
	$(CC) ${CCFLAGS} $(objects) -o $(target)

# Debug

debug: CCFLAGS += -ggdb
debug: all

# Set up

directories:
	mkdir -p $(obj_dir) $(bin_dir)

# Clean up

clean:
	rm -rf $(obj_dir) $(bin_dir)

# Misc

-include $(depends)

remake: all

.PHONY: clean
