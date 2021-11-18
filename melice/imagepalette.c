//
//  imagepalette.c
//  melice
//
//  Created by Raphaël Calabro on 01/11/2021.
//

#include "imagepalette.h"

MELListImplement(MELImagePaletteImage);

uint8_t * _Nullable MELImagePalettePaintTile(MELImagePalette * _Nonnull self, unsigned int tileIndex) {
    if (tileIndex < 0 || tileIndex >= self->super.count) {
        fprintf(stderr, "Tile index out of bounds 0..<%d: %d", self->super.count, tileIndex);
        return NULL;
    }

    const MELIntSize tileSize = self->super.tileSize;

    MELColorPalette colorPalette = *self->colorPalette;
    int *tiles = self->images[tileIndex].tiles;

    MELUInt32Color *pixels = malloc(MELPaletteByteCount(tileSize, MELIntSizeMake(1, 1)));
    const size_t count = tileSize.width * tileSize.height;
    for (size_t index = 0; index < count; index++) {
        pixels[index] = MELColorPaletteColorForTile(colorPalette, tiles[index]);
    }
    return (uint8_t *) pixels;
}

void MELImagePalettePaintTileToBuffer(MELImagePalette * _Nonnull self, unsigned int tileIndex, MELIntPoint topLeft, MELUInt32Color * _Nonnull buffer, MELIntSize bufferSize) {
    const MELIntSize tileSize = self->super.tileSize;

    MELColorPalette colorPalette = *self->colorPalette;
    int *tiles = self->images[tileIndex].tiles;

    const size_t count = tileSize.width * tileSize.height;
    for (size_t index = 0; index < count; index++) {
        const size_t x = index % tileSize.width;
        const size_t y = index / tileSize.width;
        buffer[topLeft.x + x + (topLeft.y + y) * bufferSize.width] = MELColorPaletteColorForTile(colorPalette, tiles[index]);
    }
}

uint8_t * _Nullable MELImagePalettePaintMap(MELImagePalette * _Nonnull self, MELMap map, MELIntSize areaToRender) {
    const MELIntSize tileSize = self->super.tileSize;

    MELIntSize imageSize = MELIntSizeMake(areaToRender.width * tileSize.width, areaToRender.height * tileSize.height);
    const size_t count = imageSize.width * imageSize.height;

    MELColorPalette colorPalette = *self->colorPalette;

    MELUInt32Color *pixels = malloc(MELPaletteByteCount(tileSize, areaToRender));
    for (int index = 0; index < count; index++) {
        const int x = index % imageSize.width;
        const int y = index / imageSize.width;
        const int tileX = x / tileSize.width;
        const int tileY = y / tileSize.height;
        const int tileIndex = tileX + tileY * map.size.width;

        const int xInsideTile = x % tileSize.width;
        const int yInsideTile = y % tileSize.height;
        const int indexInsideTile = xInsideTile + yInsideTile * tileSize.width;

        MELImagePaletteImage image = self->images[tileIndex];
        
        pixels[index] = MELColorPaletteColorForTile(colorPalette, image.tiles[indexInsideTile]);
    }
    return (uint8_t *) pixels;
}

MELImagePaletteImage * _Nonnull MELImagePaletteTileAtIndex(MELImagePalette * _Nonnull self,  unsigned int tileIndex) {
    return self->images + tileIndex;
}

const MELPaletteClass MELImagePaletteClass = {
    (uint8_t *(*)(MELPalette *, unsigned int)) &MELImagePalettePaintTile,
    (void(*)(MELPalette *, unsigned int, MELIntPoint, MELUInt32Color *, MELIntSize))&MELImagePalettePaintTileToBuffer,
    (uint8_t *(*)(MELPalette *, MELMap, MELIntSize)) &MELImagePalettePaintMap,
    (void *(*)(MELPalette *, unsigned int)) &MELImagePaletteTileAtIndex
};
