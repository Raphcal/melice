//
//  layer.c
//  shmup
//
//  Created by Raphaël Calabro on 30/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "layer.h"

#include <assert.h>
#include "melstring.h"
#include "melmath.h"
#include "primitives.h"

MELListImplement(MELLayer);

MELLayer MELLayerMakeWithSize(MELIntSize size) {
    const int tileCount = size.width * size.height;
    MELLayer self = {NULL, size, tileCount, calloc(tileCount, sizeof(int32_t)), MELPointMake(1, 1), false, MELSpriteInstanceListEmpty};
    for (unsigned int tile = 0; tile < tileCount; tile++) {
        self.tiles[tile] = -1;
    }
    return self;
}

MELLayer MELLayerMakeWithLayer(MELLayer other) {
    MELLayer self = other;
    self.name = MELStringCopy(other.name);
    self.tiles = MELArrayCopy(other.tiles, sizeof(int32_t) * other.tileCount);
    self.isSolid = other.isSolid;
    self.sprites = MELSpriteInstanceListMakeWithListAndCopyFunction(other.sprites, &MELSpriteInstanceMakeWithSpriteInstance);
    return self;
}

MELLayer MELLayerMakeWithInputStream(MELInputStream * _Nonnull inputStream) {
    assert(inputStream->file != NULL);

    MELLayer self;
    self.name = MELInputStreamReadString(inputStream);
    self.size = MELIntSizeMake(MELInputStreamReadInt(inputStream), MELInputStreamReadInt(inputStream));
    self.scrollRate = MELInputStreamReadPoint(inputStream);

    self.tiles = MELInputStreamReadIntArray(inputStream, &self.tileCount);
    self.sprites = MELSpriteInstanceListEmpty;
    return self;
}

MELLayer MELLayerMakeWithImagePaletteImage(MELImagePaletteImage image) {
    MELLayer self;
    self.size = image.size;
    self.tiles = image.tiles;
    return self;
}

void MELLayerDeinit(MELLayer * _Nonnull self) {
    free(self->name);
    self->name = NULL;
    self->size = MELIntSizeZero;
    self->tileCount = 0;
    free(self->tiles);
    self->tiles = NULL;
    self->scrollRate = MELPointZero;
    MELSpriteInstanceListDeinit(&self->sprites);
}

int MELLayerTileAtXAndY(MELLayer self, int x, int y) {
    return x >= 0 && x < self.size.width && y >= 0 && y < self.size.height ? self.tiles[y * self.size.width + x] : -1;
}

void MELLayerResize(MELLayer * _Nonnull self, MELIntSize newSize) {
    MELLayer layer = *self;
    if (layer.size.width == newSize.width && layer.size.height == newSize.height) {
        return;
    }

    const int newTileCount = newSize.width * newSize.height;
    int32_t *newTiles = malloc(sizeof(int32_t) * newTileCount);

    if (layer.tiles != NULL) {
        const int32_t minWidth = layer.size.width < newSize.width ? layer.size.width : newSize.width;
        const int32_t paddingWidth = newSize.width - layer.size.width;
        const int32_t minHeight = layer.size.height < newSize.height ? layer.size.height : newSize.height;

        int32_t *oldRow = layer.tiles;
        int32_t *newRow = newTiles;
        for (int32_t y = 0; y < minHeight; y++) {
            memcpy(newRow, oldRow, sizeof(int32_t) * minWidth);
            int32_t *padding = newRow + minWidth;
            for (int x = 0; x < paddingWidth; x++) {
                padding[x] = -1;
            }
            oldRow += layer.size.width;
            newRow += newSize.width;
        }
        for (int index = minHeight * newSize.width; index < newTileCount; index++) {
            newTiles[index] = -1;
        }

        free(layer.tiles);
    } else {
        for (int index = 0; index < newTileCount; index++) {
            newTiles[index] = -1;
        }
    }
    layer.tiles = newTiles;
    layer.size = newSize;
    layer.tileCount = newTileCount;
    *self = layer;
}

void MELLayerRendererToSurfaceArray(MELLayer self, MELSurfaceArray * _Nonnull destination, MELTextureAtlas textureAtlas) {
    const int width = self.size.width;
    
    for (int index = 0; index < self.tileCount; index++) {
        const GLfloat x = index % width;
        const GLfloat y = index / width;
        const int tile = self.tiles[index];
        if (tile >= 0) {
            // TODO: N'ajouter que les points nécessaire et faire des sauts quand x != oldX + 1 et y != oldY
            const MELIntSize size = textureAtlas.sources[tile].size;
            MELSurfaceArrayAppendTexturedQuad(destination, MELRectangleMake(x * size.width, y * size.height, size.width, size.height), tile, textureAtlas);
        }
    }
}

void MELLayerCopyRectangleFromOtherLayer(MELLayer self, MELIntPoint targetTopLeft, const MELLayer source, MELIntRectangle areaToCopy) {
    areaToCopy.size.width = MELFloatBound(0, areaToCopy.size.width, source.size.width - areaToCopy.origin.x);
    areaToCopy.size.height = MELFloatBound(0, areaToCopy.size.height, source.size.height - areaToCopy.origin.y);
    int32_t *targetRow = self.tiles + targetTopLeft.x + targetTopLeft.y * self.size.width;
    int32_t *sourceRow = source.tiles + areaToCopy.origin.x + areaToCopy.origin.y * source.size.width;
    size_t copySize = sizeof(int32_t) * areaToCopy.size.width;
    for (int32_t y = 0; y < areaToCopy.size.height; y++) {
        memcpy(targetRow, sourceRow, copySize);
        targetRow += self.size.width;
        sourceRow += source.size.width;
    }
}

void MELLayerMergeRectangleFromOtherLayer(MELLayer self, MELIntPoint targetTopLeft, const MELLayer source, MELIntRectangle areaToMerge) {
    int32_t *targetRow = self.tiles + targetTopLeft.x + targetTopLeft.y * self.size.width;
    int32_t *sourceRow = source.tiles + areaToMerge.origin.x + areaToMerge.origin.y * source.size.width;
    int32_t count = areaToMerge.size.width * areaToMerge.size.height;
    int32_t x, y, tile;
    for (int32_t index = 0; index < count; index++) {
        x = index % areaToMerge.size.width;
        y = index / areaToMerge.size.width;
        tile = sourceRow[x + y * source.size.width];
        if (tile != -1) {
            targetRow[x + y * self.size.width] = tile;
        }
    }
}

void MELLayerClearRectangle(MELLayer self, MELIntRectangle areaToClear) {
    int32_t *row = self.tiles + areaToClear.origin.x + areaToClear.origin.y * self.size.width;
    int32_t count = areaToClear.size.width * areaToClear.size.height;
    int32_t x, y;
    for (int32_t index = 0; index < count; index++) {
        x = index % areaToClear.size.width;
        y = index / areaToClear.size.width;
        row[x + y * self.size.width] = -1;
    }
}

MELBoolean MELLayerIsEmpty(MELLayer self) {
    return self.tiles == NULL || MELInt32ArrayIsFilledWithValue(self.tiles, self.tileCount, -1);
}
