//
//  layer.c
//  shmup
//
//  Created by Raphaël Calabro on 30/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "layer.h"

#include <assert.h>

MELLayer MELLayerMakeWithInputStream(MELInputStream * _Nonnull inputStream) {
    assert(inputStream->file != NULL);

    MELLayer self;
    self.name = MELInputStreamReadString(inputStream);
    self.width = MELInputStreamReadInt(inputStream);
    self.height = MELInputStreamReadInt(inputStream);
    self.scrollRate = MELInputStreamReadPoint(inputStream);
    
    self.tiles = MELInputStreamReadIntArray(inputStream, &self.tileCount);
    return self;
}

void MELLayerDeinit(MELLayer * _Nonnull self) {
    free(self->name);
    self->name = NULL;
    self->width = 0;
    self->height = 0;
    self->tileCount = 0;
    free(self->tiles);
    self->tiles = NULL;
    self->scrollRate = MELPointZero;
}

int MELLayerTileAtXAndY(MELLayer self, int x, int y) {
    return x >= 0 && x < self.width && y >= 0 && y < self.height ? self.tiles[y * self.width + x] : -1;
}

void MELLayerRendererToSurfaceArray(MELLayer self, MELSurfaceArray * _Nonnull destination, MELTextureAtlas textureAtlas) {
    const int width = self.width;
    
    for (int index = 0; index < self.tileCount; index++) {
        const GLfloat x = index % width;
        const GLfloat y = index / width;
        const int tile = self.tiles[index];
        if (tile >= 0) {
            // TODO: N'ajouter que les points nécessaire et faire des sauts quand x != oldX + 1 et y != oldY
            MELSurfaceArrayAppendTexturedQuad(destination, MELRectangleMake(x * MELTileSize, y * MELTileSize, MELTileSize, MELTileSize), tile, textureAtlas);
        }
    }
}
