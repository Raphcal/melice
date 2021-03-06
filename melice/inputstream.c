//
//  inputstream.c
//  shmup
//
//  Created by Raphaël Calabro on 25/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "inputstream.h"

#include <assert.h>
#include <string.h>

#define MELInputStreamBufferSize 4096

MELInputStream MELInputStreamOpen(const char * _Nonnull path) {
    return (MELInputStream) {
        fopen(path, "r"),
        malloc(sizeof(uint8_t) * MELInputStreamBufferSize),
        0,
        0
    };
}

int MELInputStreamClose(MELInputStream * _Nonnull self) {
    assert(self != NULL);
    self->cursor = 0;
    self->size = 0;
    free(self->buffer);
    self->buffer = NULL;
    FILE *file = self->file;
    self->file = NULL;
    return fclose(file);
}

size_t MELInputStreamRemaining(MELInputStream *self) {
    return self->size - self->cursor;
}

int MELInputStreamReadByte(MELInputStream *self) {
    if (self->cursor == self->size) {
        size_t read = fread(self->buffer, sizeof(uint8_t), MELInputStreamBufferSize, self->file);
        if (read < 1) {
            return -1;
        }
        self->size = read;
        self->cursor = 0;
    }
    return self->buffer[self->cursor++];
}

void MELInputStreamCompact(MELInputStream * _Nonnull self) {
    const size_t cursor = self->cursor;
    if (cursor == 0) {
        return;
    }
    const size_t remaining = self->size - cursor;
    uint8_t *buffer = self->buffer;
    memmove(buffer, buffer + cursor, remaining);
    
    self->cursor = 0;
    self->size = remaining;
}

void MELInputStreamFillBuffer(MELInputStream * _Nonnull self) {
    const size_t size = self->size;
    size_t read = fread(self->buffer + size, sizeof(uint8_t), MELInputStreamBufferSize - size, self->file);
    if (read < 1) {
        return;
    }
    self->size = size + read;
}

void MELInputStreamRead(MELInputStream * _Nonnull self, void * _Nonnull destination, size_t size) {
    if (size == 0) {
        return;
    }
    if (size > MELInputStreamBufferSize) {
        const size_t readCountFromBuffer = self->size - self->cursor;
        memcpy(destination, self->buffer + self->cursor, readCountFromBuffer);
        fread(destination + readCountFromBuffer, size - readCountFromBuffer, sizeof(uint8_t), self->file);
        self->size = 0;
        self->cursor = 0;
        return;
    }
    if (MELInputStreamRemaining(self) < size) {
        MELInputStreamCompact(self);
        MELInputStreamFillBuffer(self);
        assert(MELInputStreamRemaining(self) >= size);
    }
    const size_t cursor = self->cursor;
    memcpy(destination, self->buffer + cursor, size);
    self->cursor = cursor + size;
}

void MELInputStreamSkipBytes(MELInputStream * _Nonnull self, size_t size) {
    if (size == 0) {
        return;
    }
    if (MELInputStreamRemaining(self) < size) {
        size -= self->size - self->cursor;
        self->cursor = 0;
        self->size = 0;
        MELInputStreamFillBuffer(self);
        assert(MELInputStreamRemaining(self) >= size);
    }
    self->cursor += size;
}

MELBoolean MELInputStreamReadBoolean(MELInputStream * _Nonnull self) {
    uint8_t value;
    MELInputStreamRead(self, &value, sizeof(uint8_t));
    return (MELBoolean)value;
}

int32_t MELInputStreamReadInt(MELInputStream * _Nonnull self) {
    int32_t value;
    MELInputStreamRead(self, &value, sizeof(int32_t));
    return value;
}

int16_t MELInputStreamReadShort(MELInputStream * _Nonnull self) {
    int16_t value;
    MELInputStreamRead(self, &value, sizeof(int16_t));
    return value;
}

float MELInputStreamReadFloat(MELInputStream * _Nonnull self) {
    float value;
    MELInputStreamRead(self, &value, sizeof(float));
    return value;
}

double MELInputStreamReadDouble(MELInputStream * _Nonnull self) {
    double value;
    MELInputStreamRead(self, &value, sizeof(double));
    return value;
}

uint8_t * _Nonnull MELInputStreamReadByteArray(MELInputStream * _Nonnull self, int32_t * _Nonnull count) {
    *count = MELInputStreamReadInt(self);
    size_t size = sizeof(uint8_t) * *count;
    
    uint8_t *array = malloc(size);
    MELInputStreamRead(self, array, size);
    return array;
}

uint8_t * _Nullable MELInputStreamReadNullableByteArray(MELInputStream * _Nonnull self, int32_t * _Nonnull count) {
    if (MELInputStreamReadBoolean(self)) {
        return MELInputStreamReadByteArray(self, count);
    } else {
        *count = 0;
        return NULL;
    }
}

int32_t * _Nonnull MELInputStreamReadIntArray(MELInputStream * _Nonnull self, int32_t * _Nonnull count) {
    *count = MELInputStreamReadInt(self);
    size_t size = sizeof(int32_t) * *count;
    
    int32_t *array = malloc(size);
    MELInputStreamRead(self, array, size);
    return array;
}

wchar_t * _Nonnull MELInputStreamReadString(MELInputStream * _Nonnull self) {
    int32_t count = MELInputStreamReadInt(self);
    
    wchar_t *string = malloc(sizeof(wchar_t) * (count + 1));
    for (int32_t index = 0; index < count; index++) {
        uint16_t c;
        MELInputStreamRead(self, &c, sizeof(uint16_t));
        string[index] = (wchar_t)c;
    }
    string[count] = '\0';
    
    return string;
}

wchar_t * _Nullable MELInputStreamReadNullableString(MELInputStream * _Nonnull self) {
    if (MELInputStreamReadBoolean(self)) {
        return MELInputStreamReadString(self);
    } else {
        return NULL;
    }
}

MELColor MELInputStreamReadColor(MELInputStream * _Nonnull self) {
    return MELColorMake(MELInputStreamReadInt(self) / 255.0f, MELInputStreamReadInt(self) / 255.0f, MELInputStreamReadInt(self) / 255.0f, MELInputStreamReadInt(self) / 255.0f);
}

MELPoint MELInputStreamReadPoint(MELInputStream * _Nonnull self) {
    return MELPointMake(MELInputStreamReadFloat(self), MELInputStreamReadFloat(self));
}
