//
//  primitives.c
//  melice
//
//  Created by Raphaël Calabro on 29/10/2021.
//

#include "primitives.h"

MELListImplement(GLfloat);
MELListImplement(GLubyte);
MELListImplement(GLint);
MELListImplement(MELByte);
MELListImplement(MELBoolean);

void * _Nullable MELArrayCopy(const void * restrict _Nullable source, size_t size) {
    if (source == NULL || size == 0) {
        return NULL;
    }
    void *copy = malloc(size);
    memcpy(copy, source, size);
    return copy;
}

MELBoolean MELInt32ArrayIsFilledWithValue(const int32_t * _Nonnull self, size_t count, int32_t value) {
    for (size_t index = 0; index < count; index++) {
        if (self[index] != value) {
            return false;
        }
    }
    return true;
}

MELBoolean MELUInt32ArrayIsFilledWithValue(const uint32_t * _Nonnull self, size_t count, uint32_t value) {
    for (size_t index = 0; index < count; index++) {
        if (self[index] != value) {
            return false;
        }
    }
    return true;
}
