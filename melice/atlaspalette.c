//
//  atlaspalette.c
//  melice
//
//  Created by Raphaël Calabro on 01/12/2021.
//

#include "atlaspalette.h"

#include "bitmap.h"

MELAtlasPalette MELAtlasPaletteMake(MELTextureAtlas atlas) {
    return (MELAtlasPalette) {
        {
            &MELAtlasPaletteClass,
            NULL,
            atlas.sources != NULL
                    ? atlas.sources[0].size
                    : MELIntSizeMake(32, 32),
            atlas.frameCount
        },
        atlas
    };
}

void MELAtlasPaletteDeinit(MELAtlasPalette * _Nonnull self) {
    // Nothing to free.
}

MELAtlasPalette * _Nonnull MELAtlasPaletteRefMakeWithColorPaletteRef(MELAtlasPalette * _Nonnull other) {
    MELAtlasPalette *self = malloc(sizeof(MELAtlasPalette));
    *self = (MELAtlasPalette) {
        other->super,
        other->atlas
    };
    return self;
}

MELPaletteRef MELAtlasPaletteToPaletteRef(MELAtlasPalette self) {
    MELAtlasPalette *ref = malloc(sizeof(MELAtlasPalette));
    *ref = self;
    return &ref->super;
}

uint8_t * _Nullable MELAtlasPalettePaintTile(MELAtlasPalette * _Nonnull self, unsigned int tileIndex) {
    if (tileIndex < 0 || tileIndex >= self->super.count) {
        fprintf(stderr, "Tile index out of bounds 0..<%d: %d", self->super.count, tileIndex);
        return NULL;
    }

    MELUInt32Color *texture = self->atlas.texture.pixels;
    if (texture == NULL && self->atlas.texture.path != NULL) {
        texture = MELBitmapLoad(self->atlas.texture.path, NULL);
        self->atlas.texture.pixels = texture;
    }
    if (texture == NULL) {
        return NULL;
    }

    return (uint8_t *) MELBitmapDrawTile(self->atlas, tileIndex, texture, NULL, NULL);
}

void MELAtlasPalettePaintTileToBuffer(MELAtlasPalette * _Nonnull self, unsigned int tileIndex, MELIntPoint topLeft, MELUInt32Color * _Nonnull buffer, MELIntSize bufferSize) {
    // TODO: Not Implemented Yet.
}

uint8_t * _Nullable MELAtlasPalettePaintMap(MELAtlasPalette * _Nonnull self, MELMap map, MELIntSize areaToRender) {
    return (uint8_t *) MELBitmapDrawMap(map, self->atlas, areaToRender, NULL, NULL);
}

MELIntRectangle * _Nonnull MELAtlasPaletteTileAtIndex(MELAtlasPalette * _Nonnull self,  unsigned int tileIndex) {
    return self->atlas.sources + tileIndex;
}

const MELPaletteClass MELAtlasPaletteClass = {
    (MELPaletteRef(*)(MELPaletteRef)) &MELAtlasPaletteRefMakeWithColorPaletteRef,
    (void(*)(MELPalette *)) &MELAtlasPaletteDeinit,
    (uint8_t *(*)(MELPalette *, unsigned int)) &MELAtlasPalettePaintTile,
    (void(*)(MELPalette *, unsigned int, MELIntPoint, MELUInt32Color *, MELIntSize))&MELAtlasPalettePaintTileToBuffer,
    (uint8_t *(*)(MELPalette *, MELMap, MELIntSize)) &MELAtlasPalettePaintMap,
    (void *(*)(MELPalette *, unsigned int)) &MELAtlasPaletteTileAtIndex
};

