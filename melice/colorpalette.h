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
extern const MELPaletteRef MELPaletteDefaultColorPalette;

typedef struct {
    MELPalette super;
    MELUInt8RGBColor * _Nullable colors;
    uint8_t * _Nullable alphaLevels;
    uint8_t alphaLevelCount;
} MELColorPalette;

MELColorPalette MELColorPaletteMakeDefault(void);
MELPalette * _Nonnull MELPaletteRefAllocDefaultColorPalette(void);

MELColorPalette MELColorPaletteMakeWithColorCount(size_t colorCount);
MELColorPalette MELColorPaletteMakeWithUInt32ColorList(MELUInt32ColorList colors);
void MELColorPaletteDeinit(MELColorPalette * _Nonnull self);

MELColorPalette * _Nonnull MELColorPaletteRefMakeWithColorPaletteRef(MELColorPalette * _Nonnull other);

MELUInt32Color MELColorPaletteColorForTile(MELColorPalette self, unsigned int tileIndex);
MELUInt32Color MELColorPaletteAlphaPremultipliedColorForTile(MELColorPalette self, unsigned int tileIndex);

int MELColorPaletteTileForColor(MELColorPalette self, MELUInt32Color color);

uint8_t * _Nullable MELColorPalettePaintImage(MELColorPalette * _Nonnull self, MELImagePaletteImage image, MELBoolean premultiplyAlpha);

#endif /* colorpalette_h */
