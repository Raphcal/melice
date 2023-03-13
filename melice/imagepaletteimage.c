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
    return (MELImagePaletteImage) {
        .tiles = malloc(sizeof(int32_t) * size.width * size.height),
        .size = size,
        .decorators = MELDecoratorRefListEmpty,
    };
}

MELImagePaletteImage MELImagePaletteImageMakeWithImagePaletteImage(MELImagePaletteImage other, size_t tileCount) {
    return (MELImagePaletteImage) {
        .name = MELStringCopy(other.name),
        .tiles = MELArrayCopy(other.tiles, sizeof(int) * tileCount),
        .size = other.size,
        .decorators = MELDecoratorRefListMakeWithListAndCopyFunction(other.decorators, &MELDecoratorRefMakeWithDecoratorRef),
    };
}

void MELImagePaletteImageDeinit(MELImagePaletteImage * _Nonnull self) {
    free(self->name);
    self->name = NULL;
    free(self->tiles);
    self->tiles = NULL;
    self->size = MELIntSizeZero;
    MELDecoratorRefListDeinitWithDeinitFunction(&self->decorators, &MELDecoratorRefDeinit);
}

MELBoolean MELImagePaletteImageIsEmpty(MELImagePaletteImage self) {
    return self.tiles == NULL || MELInt32ArrayIsFilledWithValue(self.tiles, self.size.width * self.size.height, -1);
}
