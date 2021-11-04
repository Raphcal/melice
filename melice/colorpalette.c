//
//  colorpalette.c
//  melice
//
//  Created by Raphaël Calabro on 01/11/2021.
//

#include "colorpalette.h"

#define MASK 1000

MELUInt32Color MELColorPaletteColorForTile(MELColorPalette self, int tileIndex) {
    const int colorIndex = tileIndex % MASK;
    const int alphaIndex = tileIndex / MASK;

    if (colorIndex < 0 || colorIndex >= self.super.count || alphaIndex < 0 || alphaIndex >= self.alphaLevelCount) {
        return 0;
    }

    MELUint8RGBColor rgb = self.colors[colorIndex];
    uint8_t alpha = self.alphaLevels[alphaIndex];

    return MELUInt8ColorToRGBAUInt32Color(MELUInt8ColorMake(rgb.red, rgb.green, rgb.blue, alpha));
}

uint8_t * _Nullable MELColorPalettePaintTile(MELColorPalette * _Nonnull self, int tileIndex) {
    const MELIntSize tileSize = self->super.tileSize;

    MELUInt32Color color = MELColorPaletteColorForTile(*self, tileIndex);

    MELUInt32Color *pixels = malloc(MELPaletteByteCount(tileSize, MELIntSizeMake(1, 1)));
    const size_t count = tileSize.width * tileSize.height;
    for (size_t index = 0; index < count; index++) {
        pixels[index] = color;
    }
    return (uint8_t *) pixels;
}

void MELColorPalettePaintTileToBuffer(MELColorPalette * _Nonnull self, int tileIndex, MELIntPoint topLeft, MELUInt32Color * _Nonnull buffer, MELIntSize bufferSize) {
    const MELIntSize tileSize = self->super.tileSize;

    MELUInt32Color color = MELColorPaletteColorForTile(*self, tileIndex);

    const size_t count = tileSize.width * tileSize.height;
    for (size_t index = 0; index < count; index++) {
        const size_t x = index % tileSize.width;
        const size_t y = index / tileSize.width;
        buffer[topLeft.x + x + (topLeft.y + y) * bufferSize.width] = color;
    }
}

uint8_t * _Nullable MELColorPalettePaintMap(MELColorPalette * _Nonnull self, MELMap map, MELIntSize areaToRender) {
    const MELIntSize tileSize = self->super.tileSize;

    MELIntSize imageSize = MELIntSizeMake(areaToRender.width * tileSize.width, areaToRender.height * tileSize.height);
    const size_t count = imageSize.width * imageSize.height;

    MELUInt32Color *pixels = malloc(MELPaletteByteCount(tileSize, areaToRender));
    for (int index = 0; index < count; index++) {
        const int x = index % imageSize.width;
        const int y = index / imageSize.width;
        const int tileX = x / tileSize.width;
        const int tileY = y / tileSize.height;
        const int tileIndex = tileX + tileY * map.size.width;

        pixels[index] = MELColorPaletteColorForTile(*self, tileIndex);
    }
    return (uint8_t *) pixels;
}

const MELPaletteClass MELColorPaletteClass = {
    (uint8_t *(*)(MELPalette *, int)) &MELColorPalettePaintTile,
    (void(*)(MELPalette *, int, MELIntPoint, MELUInt32Color *, MELIntSize))&MELColorPalettePaintTileToBuffer,
    (uint8_t *(*)(MELPalette *, MELMap, MELIntSize)) &MELColorPalettePaintMap
};
