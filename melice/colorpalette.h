//
//  colorpalette.h
//  melice
//
//  Created by Raphaël Calabro on 01/11/2021.
//

#ifndef colorpalette_h
#define colorpalette_h

#include "melstd.h"

#include "palette.h"

extern const MELPaletteClass MELColorPaletteClass;

typedef struct {
    MELPalette super;
    MELUint8RGBColor * _Nullable colors;
    uint8_t * _Nullable alphaLevels;
    uint8_t alphaLevelCount;
} MELColorPalette;

#endif /* colorpalette_h */
