//
//  grid.c
//  shmup
//
//  Created by Raphaël Calabro on 30/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "maprenderer.h"

#include <assert.h>
#include "renderer.h"

const MELMapRenderer MELMapRendererEmpty = {};

MELMapRenderer MELMapRendererMakeWithMapAndAtlas(MELMap * _Nonnull map, MELTextureAtlas atlas) {
    return MELMapRendererMakeWithRendererAndMapAndAtlas(MELRendererGetDefault(), map, atlas);
}

MELMapRenderer MELMapRendererMakeWithRendererAndMapAndAtlas(MELRenderer * _Nonnull renderer, MELMap * _Nonnull map, MELTextureAtlas atlas) {
    MELMapRenderer self;
    self.renderer = renderer;
    self.map = map;
    self.tileSize = atlas.frameCount > 0
        ? atlas.sources[0].size
        : MELIntSizeMake(MELTileSize, MELTileSize);
    self.atlas = atlas;

    MELSurfaceArray *layerSurfaces = calloc(map->layers.count, sizeof(MELSurfaceArray));
    for (int index = 0; index < map->layers.count; index++) {
        MELLayer layer = map->layers.memory[index];
        MELSurfaceArray layerSurface = MELSurfaceArrayMakeWithInitialCapacity(layer.tileCount);

        const int width = layer.size.width;
        const int height = layer.size.height;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int tile = MELLayerTileAtXAndY(layer, x, y);
                if (tile >= 0) {
                    const MELSize size = atlas.frames[tile].size;
                    MELSurfaceArrayAppendTexturedQuad(&layerSurface, MELRectangleMake((GLfloat)x * self.tileSize.width, (GLfloat)y * self.tileSize.height, self.tileSize.width, self.tileSize.height), tile, atlas);
                }
            }
        }

        layerSurfaces[index] = layerSurface;
    }
    self.layerSurfaces = layerSurfaces;

    return self;
}

MELMapRenderer MELMapRendererMakeWithRendererAndMutableMapAndAtlas(MELRenderer * _Nonnull renderer, MELMutableMap * _Nonnull mutableMap, MELTextureAtlas atlas) {
    MELMapRenderer self = MELMapRendererMakeWithRendererAndMapAndAtlas(renderer, &mutableMap->super, atlas);
    self.tileSize = mutableMap->palette->tileSize;
    self.firstSpriteDefinitionIndex = mutableMap->palette->count;
    return self;
}

MELMapRenderer MELMapRendererMakeWithMapAndColorPalette(MELMap * _Nonnull map, MELColorPalette colorPalette) {
    MELMapRenderer self;
    self.map = map;
    self.tileSize = colorPalette.super.tileSize;
    self.firstSpriteDefinitionIndex = colorPalette.super.count;

    MELSurfaceArray *layerSurfaces = calloc(map->layers.count, sizeof(MELSurfaceArray));
    for (int index = 0; index < map->layers.count; index++) {
        MELLayer layer = map->layers.memory[index];
        MELSurfaceArray layerSurface = MELSurfaceArrayMakeWithInitialCapacity(layer.tileCount);

        const int width = layer.size.width;
        const int height = layer.size.height;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int tile = MELLayerTileAtXAndY(layer, x, y);
                if (tile >= 0) {
                    MELUInt32Color color = MELColorPaletteColorForTile(colorPalette, tile);
                    MELSurfaceArrayAppendColoredQuad(&layerSurface, MELRectangleMake((GLfloat)x * self.tileSize.width, (GLfloat)y * self.tileSize.height, self.tileSize.width, self.tileSize.height), MELRGBAUInt32ColorToMELUInt8Color(color));
                }
            }
        }

        layerSurfaces[index] = layerSurface;
    }
    self.layerSurfaces = layerSurfaces;

    return self;
}

void MELMapRendererDeinit(MELMapRenderer * _Nonnull self) {
    if (self->map != NULL) {
        const size_t layerCount = self->map->layers.count;
        MELSurfaceArray *layerSurfaces = self->layerSurfaces;
        if (layerSurfaces != NULL) {
            for (unsigned int index = 0; index < layerCount; index++) {
                MELSurfaceArrayDeinit(layerSurfaces + index);
            }
            free(layerSurfaces);
        }
    }
    self->layerSurfaces = NULL;
}

void MELMapRendererUpdate(MELMapRenderer self) {
    for (int index = 0; index < self.map->layers.count; index++) {
        MELLayer layer = self.map->layers.memory[index];
        MELSurfaceArray *layerSurface = self.layerSurfaces + index;
        layerSurface->count = 0;
        layerSurface->texture.count = 0;
        layerSurface->vertex.count = 0;
        layerSurface->color.count = 0;

        const int tileCount = layer.size.width * layer.size.height;
        for (int tileIndex = 0; tileIndex < tileCount; tileIndex++) {
            int tile = layer.tiles[tileIndex];
            if (tile >= 0) {
                GLfloat x = tileIndex % layer.size.width;
                GLfloat y = tileIndex / layer.size.width;
                MELSurfaceArrayAppendTexturedQuad(layerSurface, MELRectangleMake(x * self.tileSize.width, y * self.tileSize.height, self.tileSize.width, self.tileSize.height), tile, self.atlas);
            }
        }

        for (int spriteIndex = 0; spriteIndex < layer.sprites.count; spriteIndex++) {
            MELSpriteInstance sprite = layer.sprites.memory[spriteIndex];
            MELIntRectangle source = self.atlas.sources[self.firstSpriteDefinitionIndex + sprite.definitionIndex];
            MELSurfaceArrayAppendTexturedQuad(layerSurface, MELRectangleMakeWithOriginAndSize(sprite.topLeft, MELSizeMake(source.size.width, source.size.height)), self.firstSpriteDefinitionIndex + sprite.definitionIndex, self.atlas);
        }
    }
}

void MELMapRendererDraw(MELMapRenderer self) {
    MELMapRendererDrawRangeTranslated(self, MELPointZero, 0, (unsigned int) self.map->layers.count);
}

void MELMapRendererDrawTranslated(MELMapRenderer self, MELPoint translation) {
    MELMapRendererDrawRangeTranslated(self, translation, 0, (unsigned int) self.map->layers.count);
}

void MELMapRendererDrawTranslatedShowing(MELMapRenderer self, MELPoint translation, MELBooleanList visibilities) {
    MELRendererRefBindTexture(self.renderer, &self.atlas.texture);

    assert(visibilities.count == self.map->layers.count);

    MELSurfaceArray *layerSurfaces = self.layerSurfaces;
    MELLayer *layers = self.map->layers.memory;
    for (unsigned int index = 0; index < self.map->layers.count; index++) {
        if (visibilities.memory[index]) {
            MELRendererRefTranslateToTopLeft(self.renderer, MELPointMultiply(translation, layers[index].scrollRate));

            MELSurfaceArray layerSurface = layerSurfaces[index];
            MELRendererRefDrawWithVertexPointerAndTexCoordPointer(self.renderer, MELCoordinatesByVertex, (GLfloat *) layerSurface.vertex.memory, MELCoordinatesByTexture, (GLfloat *) layerSurface.texture.memory, layerSurface.count);
        }
    }
}

void MELMapRendererDrawRangeTranslated(MELMapRenderer self, MELPoint translation, unsigned int fromLayer, unsigned int toLayer) {
    MELRendererRefBindTexture(self.renderer, &self.atlas.texture);

    if (self.map->layers.count == 0) {
        return;
    }

    assert(fromLayer >= 0);
    assert(toLayer <= (unsigned int)self.map->layers.count);

    MELSurfaceArray *layerSurfaces = self.layerSurfaces;
    MELLayer *layers = self.map->layers.memory;
    for (unsigned int index = fromLayer; index < toLayer; index++) {
        const MELPoint layerTranslation = MELPointMultiply(translation, layers[index].scrollRate);

        MELRendererRefTranslateToTopLeft(self.renderer, layerTranslation);
        MELSurfaceArray layerSurface = layerSurfaces[index];
        MELRendererRefDrawWithVertexPointerAndTexCoordPointer(self.renderer, MELCoordinatesByVertex, (GLfloat *) layerSurface.vertex.memory, MELCoordinatesByTexture, (GLfloat *) layerSurface.texture.memory, layerSurface.count);
    }
}

void MELMapRendererDrawColorRangeTranslated(MELMapRenderer self, MELPoint translation, unsigned int fromLayer, unsigned int toLayer) {
    assert(fromLayer >= 0);
    assert(toLayer <= (unsigned int)self.map->layers.count);
    
    MELSurfaceArray *layerSurfaces = self.layerSurfaces;
    MELLayer *layers = self.map->layers.memory;
    for (unsigned int index = fromLayer; index < toLayer; index++) {
        const MELPoint layerTranslation = MELPointMultiply(translation, layers[index].scrollRate);
        
        MELRendererRefTranslateToTopLeft(self.renderer, layerTranslation);
        MELSurfaceArray layerSurface = layerSurfaces[index];
        MELRendererRefDrawWithVertexPointerAndColorPointer(self.renderer, MELCoordinatesByVertex, (GLfloat *) layerSurface.vertex.memory, MELCoordinatesByColor, (GLubyte *) layerSurface.color.memory, layerSurface.count);
    }
}
