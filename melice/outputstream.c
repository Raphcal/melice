//
//  outputstream.c
//  melice
//
//  Created by Raphaël Calabro on 22/11/2021.
//

#include "outputstream.h"

#include <assert.h>
#include "melstring.h"

#define MELOutputStreamBufferSize 4096

MELOutputStream MELOutputStreamOpen(const char * _Nonnull path) {
    return (MELOutputStream) {
        fopen(path, "wb"),
        malloc(sizeof(uint8_t) * MELOutputStreamBufferSize),
        0,
        MELOutputStreamBufferSize
    };
}

MELOutputStream MELOutputStreamInit(void) {
    return (MELOutputStream) {
        NULL,
        malloc(sizeof(uint8_t) * MELOutputStreamBufferSize),
        0,
        MELOutputStreamBufferSize
    };
}

int MELOutputStreamClose(MELOutputStream * _Nonnull self) {
    MELOutputStreamFlush(self);

    assert(self != NULL);
    self->count = 0;
    free(self->buffer);
    self->buffer = NULL;

    if (self->file) {
        FILE *file = self->file;
        self->file = NULL;
        return fclose(file);
    } else {
        return 0;
    }
}

void MELOutputStreamDeinit(MELOutputStream * _Nonnull self) {
    MELOutputStreamClose(self);
}

uint8_t * _Nonnull MELOutputStreamGetBytes(MELOutputStream self) {
    uint8_t * bytes = malloc(sizeof(uint8_t) * self.count);
    memcpy(bytes, self.buffer, sizeof(uint8_t) * self.count);
    return bytes;
}

void MELOutputStreamFlush(MELOutputStream * _Nonnull self) {
    if (self->file && self->count > 0) {
        fwrite(self->buffer, sizeof(uint8_t), self->count, self->file);
        self->count = 0;
    } else if (!self->file && self->count >= self->capacity) {
        self->buffer = realloc(self->buffer, sizeof(uint8_t) * self->capacity * 2);
        self->capacity *= 2;
    }
}

void MELOutputStreamWriteByte(MELOutputStream * _Nonnull self, uint8_t byte) {
    self->buffer[self->count++] = byte;
    if (self->count == self->capacity) {
        MELOutputStreamFlush(self);
    }
}

void MELOutputStreamWrite(MELOutputStream * _Nonnull self, void * _Nonnull source, size_t size) {
    size_t count;
    do {
        count = self->count + size > self->capacity ? self->capacity - self->count : size;
        memcpy(self->buffer + self->count, source, count);
        self->count += count;
        if (self->count == self->capacity) {
            MELOutputStreamFlush(self);
        }
        size -= count;
        source += count;
    } while (size > 0);
}

void MELOutputStreamWriteNumeric(MELOutputStream * _Nonnull self, int64_t value, size_t byteCount) {
    for(size_t index = 0; index < byteCount; index++) {
        MELOutputStreamWriteByte(self, value & 0xFF);
        value = value >> 8;
    }
}

void MELOutputStreamWriteZero(MELOutputStream * _Nonnull self, size_t size) {
    uint8_t zeroes[size];
    memset(zeroes, 0, sizeof(uint8_t) * size);
    MELOutputStreamWrite(self, zeroes, sizeof(uint8_t) * size);
}

void MELOutputStreamWriteBoolean(MELOutputStream * _Nonnull self, MELBoolean value) {
    MELOutputStreamWriteByte(self, value);
}

void MELOutputStreamWriteInt(MELOutputStream * _Nonnull self, int32_t value) {
    MELOutputStreamWriteNumeric(self, value, sizeof(int32_t));
}

void MELOutputStreamWriteShort(MELOutputStream * _Nonnull self, int16_t value) {
    MELOutputStreamWriteNumeric(self, value, sizeof(int16_t));
}

void MELOutputStreamWriteUInt16(MELOutputStream * _Nonnull self, uint16_t value) {
    MELOutputStreamWriteNumeric(self, value, sizeof(uint16_t));
}

void MELOutputStreamWriteUInt32(MELOutputStream * _Nonnull self, uint32_t value) {
    MELOutputStreamWriteNumeric(self, value, sizeof(uint32_t));
}

void MELOutputStreamWriteFloat(MELOutputStream * _Nonnull self, float value) {
    MELOutputStreamWriteNumeric(self, *((int32_t *) &value), sizeof(float));
}

void MELOutputStreamWriteDouble(MELOutputStream * _Nonnull self, double value) {
    MELOutputStreamWriteNumeric(self, *((int64_t *) &value), sizeof(double));
}

void MELOutputStreamWriteByteArray(MELOutputStream * _Nonnull self, uint8_t * _Nonnull value, int32_t count) {
    MELOutputStreamWriteInt(self, count);
    MELOutputStreamWrite(self, value, sizeof(uint8_t) * count);
}

void MELOutputStreamWriteNullableByteArray(MELOutputStream * _Nonnull self, uint8_t * _Nullable value, int32_t count) {
    MELOutputStreamWriteBoolean(self, value != NULL);
    if (value != NULL) {
        MELOutputStreamWriteByteArray(self, value, count);
    }
}

void MELOutputStreamWriteIntArray(MELOutputStream * _Nonnull self, int32_t * _Nonnull value, int32_t count) {
    MELOutputStreamWriteInt(self, count);
    for (int32_t index = 0; index < count; index++) {
        MELOutputStreamWriteInt(self, value[index]);
    }
}

size_t str16len(uint16_t * _Nonnull array) {
    uint16_t *current;
    for (current = array; *current; current++);
    return current - array;
}

void MELOutputStreamWriteString(MELOutputStream * _Nonnull self, char * _Nonnull value) {
    uint16_t *utf16String = MELUTF16StringMakeWithUTF8String(value);
    size_t count = str16len(utf16String);

    MELOutputStreamWriteInt(self, (int32_t) count);
    for (size_t index = 0; index < count; index++) {
        MELOutputStreamWriteUInt16(self, utf16String[index]);
    }
}

void MELOutputStreamWriteNullableString(MELOutputStream * _Nonnull self, char * _Nullable value) {
    MELOutputStreamWriteBoolean(self, value != NULL);
    if (value != NULL) {
        MELOutputStreamWriteString(self, value);
    }
}

void MELOutputStreamWriteColor(MELOutputStream * _Nonnull self, MELColor value) {
    MELUInt8Color color = MELColorToMELUInt8Color(value);
    MELOutputStreamWriteInt(self, color.red);
    MELOutputStreamWriteInt(self, color.green);
    MELOutputStreamWriteInt(self, color.blue);
    MELOutputStreamWriteInt(self, color.alpha);
}

void MELOutputStreamWritePoint(MELOutputStream * _Nonnull self, MELPoint value) {
    MELOutputStreamWriteFloat(self, value.x);
    MELOutputStreamWriteFloat(self, value.y);
}
