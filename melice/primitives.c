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

void * _Nullable MELArrayCopy(const void * restrict _Nullable source, size_t size) {
    if (source == NULL || size == 0) {
        return NULL;
    }
    void *copy = malloc(size);
    memcpy(copy, source, size);
    return copy;
}
