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

MELMapRenderer MELMapRendererMakeWithMapAndAtlas(MELMap map, MELTextureAtlas atlas) {
    return MELMapRendererMakeWithRendererAndMapAndAtlas(MELRendererGetDefault(), map, atlas);
}

MELMapRenderer MELMapRendererMakeWithRendererAndMapAndAtlas(MELRenderer * _Nonnull renderer, MELMap map, MELTextureAtlas atlas) {
    MELMapRenderer self;
    self.renderer = renderer;
    self.map = map;
    self.atlas = atlas;

    MELSurfaceArray *layerSurfaces = calloc(map.layerCount, sizeof(MELSurfaceArray));
    for (int index = 0; index < map.layerCount; index++) {
        MELLayer layer = map.layers[index];
        MELSurfaceArray layerSurface = MELSurfaceArrayMakeWithInitialCapacity(layer.tileCount);

        const int width = layer.size.width;
        const int height = layer.size.height;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int tile = MELLayerTileAtXAndY(layer, x, y);
                if (tile >= 0) {
                    MELSurfaceArrayAppendTexturedQuad(&layerSurface, MELRectangleMake((GLfloat)x * MELTileSize, (GLfloat)y * MELTileSize, MELTileSize, MELTileSize), tile, atlas);
                }
            }
        }

        layerSurfaces[index] = layerSurface;
    }
    self.layerSurfaces = layerSurfaces;

    return self;
}

MELMapRenderer MELMapRendererMakeWithMapAndPalette(MELMap map) {
    // TODO: Finish this method (ColorPalette is missing).
    printf("Not implented yet\n");
    MELMapRenderer self;
    self.map = map;

    MELUInt8Color black = MELUInt8ColorMake(0, 0, 0, 255);

    MELSurfaceArray *layerSurfaces = calloc(map.layerCount, sizeof(MELSurfaceArray));
    for (int index = 0; index < map.layerCount; index++) {
        MELLayer layer = map.layers[index];
        MELSurfaceArray layerSurface = MELSurfaceArrayMakeWithInitialCapacity(layer.tileCount);

        const int width = layer.size.width;
        const int height = layer.size.height;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int tile = MELLayerTileAtXAndY(layer, x, y);
                if (tile >= 0) {
                    MELSurfaceArrayAppendColoredQuad(&layerSurface, MELRectangleMake((GLfloat)x * MELTileSize, (GLfloat)y * MELTileSize, MELTileSize, MELTileSize), black);
                }
            }
        }

        layerSurfaces[index] = layerSurface;
    }
    self.layerSurfaces = layerSurfaces;

    return self;
}

void MELMapRendererDeinit(MELMapRenderer * _Nonnull self) {
    const unsigned int layerCount = self->map.layerCount;
    MELSurfaceArray *layerSurfaces = self->layerSurfaces;
    for (unsigned int index = 0; index < layerCount; index++) {
        MELSurfaceArrayDeinit(layerSurfaces + index);
    }
    free(layerSurfaces);
    self->layerSurfaces = NULL;
}

void MELMapRendererDraw(MELMapRenderer self) {
    MELMapRendererDrawRangeTranslated(self, MELPointZero, 0, self.map.layerCount);
}

void MELMapRendererDrawTranslated(MELMapRenderer self, MELPoint translation) {
    MELMapRendererDrawRangeTranslated(self, translation, 0, self.map.layerCount);
}

void MELMapRendererDrawRangeTranslated(MELMapRenderer self, MELPoint translation, unsigned int fromLayer, unsigned int toLayer) {
    MELRendererRefBindTexture(self.renderer, &self.atlas.texture);

    assert(fromLayer >= 0);
    assert(toLayer <= (unsigned int)self.map.layerCount);

    MELSurfaceArray *layerSurfaces = self.layerSurfaces;
    MELLayer *layers = self.map.layers;
    for (unsigned int index = fromLayer; index < toLayer; index++) {
        const MELPoint layerTranslation = MELPointMultiply(translation, layers[index].scrollRate);

        MELRendererRefTranslateToTopLeft(self.renderer, layerTranslation);
        MELSurfaceArray layerSurface = layerSurfaces[index];
        MELRendererRefDrawWithVertexPointerAndTexCoordPointer(self.renderer, MELCoordinatesByVertex, (GLfloat *) layerSurface.vertex.memory, MELCoordinatesByTexture, (GLfloat *) layerSurface.texture.memory, layerSurface.count);
    }
}

void MELMapRendererDrawColorRangeTranslated(MELMapRenderer self, MELPoint translation, unsigned int fromLayer, unsigned int toLayer) {
    assert(fromLayer >= 0);
    assert(toLayer <= (unsigned int)self.map.layerCount);
    
    MELSurfaceArray *layerSurfaces = self.layerSurfaces;
    MELLayer *layers = self.map.layers;
    for (unsigned int index = fromLayer; index < toLayer; index++) {
        const MELPoint layerTranslation = MELPointMultiply(translation, layers[index].scrollRate);
        
        MELRendererRefTranslateToTopLeft(self.renderer, layerTranslation);
        MELSurfaceArray layerSurface = layerSurfaces[index];
        MELRendererRefDrawWithVertexPointerAndColorPointer(self.renderer, MELCoordinatesByVertex, (GLfloat *) layerSurface.vertex.memory, MELCoordinatesByColor, (GLubyte *) layerSurface.color.memory, layerSurface.count);
    }
}
