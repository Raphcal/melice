//
//  atlaspalette.h
//  melice
//
//  Created by Raphaël Calabro on 01/12/2021.
//

#ifndef atlaspalette_h
#define atlaspalette_h

#include "melstd.h"

#include "palette.h"
#include "textureatlas.h"

extern const MELPaletteClass MELAtlasPaletteClass;

typedef struct {
    MELPalette super;
    MELTextureAtlas atlas;
} MELAtlasPalette;

MELAtlasPalette MELAtlasPaletteMake(MELTextureAtlas atlas);
void MELAtlasPaletteDeinit(MELAtlasPalette * _Nonnull self);

MELPalette * _Nonnull MELAtlasPaletteToPaletteRef(MELAtlasPalette self);

#endif /* atlaspalette_h */
