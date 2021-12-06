//
//  imagepaletteimage.h
//  melice
//
//  Created by Raphaël Calabro on 07/12/2021.
//

#ifndef imagepaletteimage_h
#define imagepaletteimage_h

#include "melstd.h"

#include "decorator.h"

typedef struct {
    int * _Nullable tiles;
    MELDecoratorRefList decorators;
} MELImagePaletteImage;

MELListDefine(MELImagePaletteImage);

void MELImagePaletteImageDeinit(MELImagePaletteImage * _Nonnull self);

#endif /* imagepaletteimage_h */
