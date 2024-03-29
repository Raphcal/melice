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
#include "size.h"

typedef struct {
    char * _Nullable name;
    int * _Nullable tiles;
    MELIntSize size;
    MELDecoratorRefList decorators;
} MELImagePaletteImage;

MELListDefine(MELImagePaletteImage);

extern const MELImagePaletteImage MELImagePaletteImageEmpty;

MELImagePaletteImage MELImagePaletteImageMakeWithSize(MELIntSize size);
MELImagePaletteImage MELImagePaletteImageMakeWithImagePaletteImage(MELImagePaletteImage other, size_t tileCount);

void MELImagePaletteImageDeinit(MELImagePaletteImage * _Nonnull self);

MELBoolean MELImagePaletteImageIsEmpty(MELImagePaletteImage self);

#endif /* imagepaletteimage_h */
