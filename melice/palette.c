//
//  palette.c
//  melice
//
//  Created by Raphaël Calabro on 02/11/2021.
//

#include "palette.h"

MELListImplement(MELPaletteRef);

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
    return other->class->initWithPaletteRef(other);
}

void MELPaletteRefDeinit(MELPaletteRef * _Nonnull self) {
    MELPaletteDeinit(*self);
    free(*self);
    *self = NULL;
}

size_t MELPaletteByteCount(MELIntSize tileSize, MELIntSize imageSize) {
    return sizeof(MELUInt32Color) * tileSize.width * imageSize.width * tileSize.height * imageSize.height;
}
