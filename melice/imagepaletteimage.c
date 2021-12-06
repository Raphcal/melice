//
//  imagepaletteimage.c
//  melice
//
//  Created by Raphaël Calabro on 07/12/2021.
//

#include "imagepaletteimage.h"

MELListImplement(MELImagePaletteImage);

void MELImagePaletteImageDeinit(MELImagePaletteImage * _Nonnull self) {
    free(self->tiles);
    self->tiles = NULL;
    MELDecoratorRefListDeinitWithDeinitFunction(&self->decorators, &MELDecoratorRefDeinit);
}
