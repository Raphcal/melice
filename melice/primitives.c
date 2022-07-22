//
//  primitives.c
//  melice
//
//  Created by Raphaël Calabro on 29/10/2021.
//

#include "primitives.h"

#include "melmath.h"

MELListImplement(GLfloat);
MELListImplement(GLubyte);
MELListImplement(GLint);
MELListImplement(MELByte);
MELListImplement(MELBoolean);
MELListImplement(MELRef);

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

MELDegrees MELDegreesForSimplifiedAngle(double simplifiedAngle) {
    switch ((int)(simplifiedAngle * 100)) {
        case 0:
            return 0;
        case 78:
            return 45;
        case 157:
            return 90;
        case 235:
            return 135;
        case 314:
            return 180;
        case 392:
            return 225;
        case 471:
            return 270;
        case 549:
            return 315;
        default:
            fprintf(stderr, "Unsupported simplified angle: %f\n", simplifiedAngle);
            return 0;
    }
}

double MELSimplifiedAngleForDegrees(MELDegrees degrees) {
    return ((int)(MELDegreesToRadians(degrees) * 100.0)) / 100.0;
}
