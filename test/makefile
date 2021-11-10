MELICE_ROOT = ..
MELICE_LIB = $(MELICE_ROOT)/dist/melice.a
MELICE_HEADERS = $(MELICE_ROOT)/dist/headers

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

SOURCES = $(call rwildcard,./,*.c)
TESTS = $(patsubst %.c, %, $(SOURCES))
DSYM = $(patsubst %.c, %.dSYM, $(SOURCES))

ifeq ($(shell uname -s),Darwin)
CFLAGS = -fsanitize=address -fsanitize=undefined -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls -F/Library/Frameworks -framework SDL2 -framework OpenGL
endif

ifeq ($(shell uname -s),Linux)
OS = linux
CFLAGS = $(shell pkg-config sdl2 gl --cflags) -I$(MELICE_HEADERS) -Wall
endif

.PHONY: all

all: $(TESTS)
	$(info Done compiling $(TESTS))

clean:
	$(RM) $(TESTS)
	$(RM) -r $(DSYM)

test: $(TESTS) FORCE
	$(foreach test, $(TESTS), echo $(test); MallocNanoZone=0 $(test); echo "";)

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(MELICE_LIB) -I $(MELICE_HEADERS) -g

FORCE:
