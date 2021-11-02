//
//  imagepalette.h
//  melice
//
//  Created by Raphaël Calabro on 01/11/2021.
//

#ifndef imagepalette_h
#define imagepalette_h

#include "melstd.h"

#include "palette.h"

extern const MELPaletteClass MELImagePaletteClass;

typedef struct {
    int * _Nullable tiles;
    MELDecorator * _Nullable decorators;
} MELImagePaletteImage;

typedef struct {
    MELPalette super;
    MELPalette * _Nullable palette;
    MELImagePaletteImage * _Nullable images;
} MELImagePalette;

MELListDefine(MELImagePaletteImage);

#endif /* imagepalette_h */
