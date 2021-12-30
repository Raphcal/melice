//
//  imagepaletteimage.c
//  melice
//
//  Created by Raphaël Calabro on 07/12/2021.
//

#include "imagepaletteimage.h"

#include "primitives.h"

MELListImplement(MELImagePaletteImage);

const MELImagePaletteImage MELImagePaletteImageEmpty = {};

MELImagePaletteImage MELImagePaletteImageMakeWithSize(MELIntSize size) {
    MELImagePaletteImage self;
    self.tiles = malloc(sizeof(int32_t) * size.width * size.height);
    self.size = size;
    self.decorators = MELDecoratorRefListEmpty;
    return self;
}

MELImagePaletteImage MELImagePaletteImageMakeWithImagePaletteImage(MELImagePaletteImage other, size_t tileCount) {
    MELImagePaletteImage self = other;
    self.tiles = MELArrayCopy(other.tiles, sizeof(int) * tileCount);
    self.decorators = MELDecoratorRefListMakeWithListAndCopyFunction(other.decorators, &MELDecoratorRefMakeWithDecoratorRef);
    return self;
}

void MELImagePaletteImageDeinit(MELImagePaletteImage * _Nonnull self) {
    free(self->tiles);
    self->tiles = NULL;
    self->size = MELIntSizeZero;
    MELDecoratorRefListDeinitWithDeinitFunction(&self->decorators, &MELDecoratorRefDeinit);
}

MELBoolean MELImagePaletteImageIsEmpty(MELImagePaletteImage self) {
    return self.tiles == NULL || MELInt32ArrayIsFilledWithValue(self.tiles, self.size.width * self.size.height, -1);
}
