//
//  imagepalette.c
//  melice
//
//  Created by Raphaël Calabro on 01/11/2021.
//

#include "imagepalette.h"

#define DEFAULT_EMPTY_IMAGE_COUNT 4
#define DEFAULT_TILE_SIZE ((MELIntSize){32, 32})

const MELImagePalette MELImagePaletteEmpty = {{&MELImagePaletteClass, NULL, DEFAULT_TILE_SIZE, 6, 0}};

MELImagePalette MELImagePaletteMakeWithColorPalette(MELColorPalette * _Nonnull colorPalette) {
    MELImagePalette self;
    self.super.class = &MELImagePaletteClass;
    self.super.count = DEFAULT_EMPTY_IMAGE_COUNT;
    self.super.name = strdup("New image palette");
    self.super.columns = 6;
    self.super.tileSize = DEFAULT_TILE_SIZE;
    self.images = malloc(DEFAULT_EMPTY_IMAGE_COUNT * sizeof(MELImagePaletteImage));
    const size_t tileCount = self.super.tileSize.width * self.super.tileSize.height;
    for (unsigned int index = 0; index < DEFAULT_EMPTY_IMAGE_COUNT; index++) {
        MELImagePaletteImage image = {
            .tiles = malloc(tileCount * sizeof(int32_t)),
            .size = DEFAULT_TILE_SIZE,
            .decorators = MELDecoratorRefListEmpty,
        };
        for (unsigned int tile = 0; tile < tileCount; tile++) {
            image.tiles[tile] = -1;
        }
        self.images[index] = image;
    }
    self.colorPalette = colorPalette;
    return self;
}

void MELImagePaletteDeinit(MELImagePalette * _Nonnull self) {
    for (uint32_t index = 0; index < self->super.count; index++) {
        MELImagePaletteImageDeinit(self->images + index);
    }
    free(self->images);
    self->images = NULL;
}

MELImagePalette * _Nonnull MELImagePaletteRefMakeWithMELImagePaletteRef(MELImagePalette * _Nonnull other) {
    MELImagePalette *self = malloc(sizeof(MELImagePalette));
    *self = *other;

    const size_t count = other->super.count;
    self->images = malloc(sizeof(MELImagePaletteImage) * count);

    const size_t tileCount = other->super.tileSize.width * other->super.tileSize.height;
    for (unsigned int index = 0; index < count; index++) {
        self->images[index] = MELImagePaletteImageMakeWithImagePaletteImage(other->images[index], tileCount);
    }
    return self;
}

uint8_t * _Nullable MELImagePalettePaintTile(MELImagePalette * _Nonnull self, unsigned int tileIndex) {
    if (tileIndex < 0 || tileIndex >= self->super.count) {
        fprintf(stderr, "Tile index out of bounds 0..<%d: %d", self->super.count, tileIndex);
        return NULL;
    }
    return MELColorPalettePaintImage(self->colorPalette, self->images[tileIndex], false);
}

void MELImagePalettePaintTileToBuffer(MELImagePalette * _Nonnull self, unsigned int tileIndex, MELIntPoint topLeft, MELUInt32Color * _Nonnull buffer, MELIntSize bufferSize) {
    const MELIntSize tileSize = self->super.tileSize;

    MELColorPalette colorPalette = *self->colorPalette;
    int *tiles = self->images[tileIndex].tiles;

    const size_t count = tileSize.width * tileSize.height;
    for (size_t index = 0; index < count; index++) {
        const size_t x = index % tileSize.width;
        const size_t y = index / tileSize.width;
        buffer[topLeft.x + x + (topLeft.y + y) * bufferSize.width] = MELColorPaletteAlphaPremultipliedColorForTile(colorPalette, tiles[index]);
    }
}

uint8_t * _Nullable MELImagePalettePaintMap(MELImagePalette * _Nonnull self, MELMap map, MELIntSize areaToRender) {
    const MELColorPalette colorPalette = *self->colorPalette;

    const MELIntSize tileSize = self->super.tileSize;
    const MELIntSize totalSize = MELIntSizeMake(areaToRender.width * tileSize.width, areaToRender.height * tileSize.height);
    const size_t pixelCount = totalSize.width * totalSize.height;

    MELUInt8Color *pixels = malloc(sizeof(MELUInt8Color) * pixelCount);
    for (int index = 0; index < pixelCount; index++) {
        const int x = index % totalSize.width;
        const int y = index / totalSize.width;
        const int tileX = x / tileSize.width;
        const int tileY = y / tileSize.height;

        const int xInsideTile = x % tileSize.width;
        const int yInsideTile = y % tileSize.height;
        const int indexInsideTile = xInsideTile + yInsideTile * tileSize.width;

        MELUInt8Color color = MELColorToMELUInt8Color(map.backgroundColor);
        for (int layerIndex = 0; layerIndex < map.layers.count; layerIndex++) {
            const MELLayer layer = map.layers.memory[layerIndex];
            if (tileX >= layer.size.width || tileY >= layer.size.height) {
                continue;
            }
            const int tileIndex = tileX + tileY * layer.size.width;
            const int tile = layer.tiles[tileIndex];
            if (tile != -1) {
                MELImagePaletteImage image = self->images[tile];
                const MELUInt32Color tileColor = MELColorPaletteColorForTile(colorPalette, image.tiles[indexInsideTile]);

                color = MELUInt8ColorBlendWithColor(color, MELRGBAUInt32ColorToMELUInt8Color(tileColor));
            }
        }
        pixels[index] = color;
    }
    return (uint8_t *) pixels;
}

MELImagePaletteImage * _Nonnull MELImagePaletteTileAtIndex(MELImagePalette * _Nonnull self,  unsigned int tileIndex) {
    return self->images + tileIndex;
}

MELBoolean MELPaletteIsImagePalette(MELPalette * _Nullable self) {
    return self != NULL && self->class == &MELImagePaletteClass;
}

const MELPaletteClass MELImagePaletteClass = {
    .initWithPaletteRef = (MELPaletteRef(*)(MELPaletteRef)) &MELImagePaletteRefMakeWithMELImagePaletteRef,
    .deinit = (void(*)(MELPalette *)) &MELImagePaletteDeinit,
    .paintTile = (uint8_t *(*)(MELPalette *, unsigned int)) &MELImagePalettePaintTile,
    .paintTileToBuffer = (void(*)(MELPalette *, unsigned int, MELIntPoint, MELUInt32Color *, MELIntSize))&MELImagePalettePaintTileToBuffer,
    .paintImage = NULL,
    .paintMap = (uint8_t *(*)(MELPalette *, MELMap, MELIntSize)) &MELImagePalettePaintMap,
    .tileAtIndex = (void *(*)(MELPalette *, unsigned int)) &MELImagePaletteTileAtIndex
};
