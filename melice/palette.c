//
//  palette.c
//  melice
//
//  Created by Raphaël Calabro on 02/11/2021.
//

#include "palette.h"

MELListImplement(MELPaletteRef);
MELListImplementIndexOf(MELPaletteRef);

void MELPaletteDeinit(MELPalette * _Nonnull self) {
    self->class->deinit(self);
    free(self->name);
    self->name = NULL;
    self->tileSize = MELIntSizeZero;
    self->columns = 0;
    self->count = 0;
}

MELPaletteRef MELPaletteRefMakeWithPaletteRef(MELPaletteRef other) {
    if (other == NULL) {
        return NULL;
    }
    MELPaletteRef self = other->class->initWithPaletteRef(other);
    self->name = MELStringCopy(other->name);
    self->tileSize = other->tileSize;
    self->columns = other->columns;
    self->count = other->count;
    return self;
}

void MELPaletteRefDeinit(MELPaletteRef * _Nonnull self) {
    MELPaletteDeinit(*self);
    free(*self);
    *self = NULL;
}

size_t MELPaletteByteCount(MELIntSize tileSize, MELIntSize imageSize) {
    return sizeof(MELUInt32Color) * tileSize.width * imageSize.width * tileSize.height * imageSize.height;
}

MELBoolean MELPaletteRefEquals(MELPaletteRef lhs, MELPaletteRef rhs) {
    if (lhs == NULL) {
        return rhs == NULL;
    }
    if (rhs == NULL) {
        return false;
    }
    if (lhs->class != rhs->class) {
        return false;
    }
    if (lhs->count != rhs->count) {
        return false;
    }
    if (lhs->name == NULL) {
        return rhs->name == NULL;
    }
    if (rhs->name == NULL) {
        return false;
    }
    return MELStringEquals(lhs->name, rhs->name);
}
