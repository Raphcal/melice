//
//  imagepaletteimage.c
//  melice
//
//  Created by Raphaël Calabro on 07/12/2021.
//

#include "imagepaletteimage.h"

MELListImplement(MELImagePaletteImage);

MELImagePaletteImage MELImagePaletteImageMakeWithImagePaletteImage(MELImagePaletteImage other, size_t tileCount) {
    MELImagePaletteImage self = other;
    self.tiles = MELArrayCopy(other.tiles, sizeof(int) * tileCount);
    self.decorators = MELDecoratorRefListMakeWithListAndCopyFunction(other.decorators, &MELDecoratorRefMakeWithDecoratorRef);
    return self;
}

void MELImagePaletteImageDeinit(MELImagePaletteImage * _Nonnull self) {
    free(self->tiles);
    self->tiles = NULL;
    MELDecoratorRefListDeinitWithDeinitFunction(&self->decorators, &MELDecoratorRefDeinit);
}
