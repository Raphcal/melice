TARGET = melice.a
SOURCE_FOLDER = melice
MINIZ_FOLDER = dependencies/miniz
BUILD_FOLDER = dist

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

SOURCES = $(call rwildcard,$(SOURCE_FOLDER)/,*.c) $(wildcard $(MINIZ_FOLDER)/*.c)
OBJECTS = $(patsubst $(SOURCE_FOLDER)/%.c, $(BUILD_FOLDER)/obj/%.o, $(SOURCES)) $(patsubst $(MINIZ_FOLDER)/%.c, $(BUILD_FOLDER)/obj/%.o, $(SOURCES))

SOURCE_HEADERS = $(call rwildcard,$(SOURCE_FOLDER)/,*.h)
DIST_HEADERS = $(patsubst $(SOURCE_FOLDER)/%.h, $(BUILD_FOLDER)/headers/%.h, $(SOURCE_HEADERS))

ifeq ($(shell uname -s),Darwin)
CFLAGS = -F$(HOME)/Library/Frameworks -Wall -Wno-initializer-overrides

ifeq ($(DEBUG),1)
TARGET = melice-debug.a
CFLAGS = -F$(HOME)/Library/Frameworks -Wall -Wno-initializer-overrides -O0 -g
endif

endif

ifeq ($(shell uname -s),Linux)
CFLAGS = $(shell pkg-config sdl2 gl --cflags)
endif

.PHONY: all

all: $(BUILD_FOLDER)/$(TARGET) $(DIST_HEADERS)

headers: $(DIST_HEADERS)

clean:
	$(RM) -r $(BUILD_FOLDER)

$(BUILD_FOLDER)/obj/%.o: $(SOURCE_FOLDER)/%.c
	mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_FOLDER)/obj/%.o: $(MINIZ_FOLDER)/%.c
	mkdir -p "$(@D)"
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_FOLDER)/headers/%.h: $(SOURCE_FOLDER)/%.h
	mkdir -p "$(@D)"
	cp $< $@

$(BUILD_FOLDER)/$(TARGET): $(OBJECTS)
	ar rcs $@ $^
