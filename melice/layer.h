//
//  layer.h
//  shmup
//
//  Created by Raphaël Calabro on 30/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef layer_h
#define layer_h

#include "melstd.h"
#include "point.h"
#include "inputstream.h"
#include "surfacearray.h"
#include "textureatlas.h"
#include "spriteinstance.h"
#include "list.h"

typedef struct {
    char * _Nullable name;
    MELIntSize size;
    int tileCount;
    int32_t * _Nullable tiles;
    MELPoint scrollRate;
    MELSpriteInstanceList sprites;
} MELLayer;

MELListDefine(MELLayer);

MELLayer MELLayerMakeWithSize(MELIntSize size);

MELLayer MELLayerMakeWithLayer(MELLayer other);

/**
 * Returns a layer with the content of the given input stream.
 *
 * @return A layer.
 */
MELLayer MELLayerMakeWithInputStream(MELInputStream * _Nonnull inputStream);

/**
 * Deinitialize and free the resources used by the given layer.
 * The layer itself is not freed.
 *
 * @param self Layer instance.
 */
void MELLayerDeinit(MELLayer * _Nonnull self);

/**
 * Returns the tile located at the given coordinates in the given layer.
 *
 * @param self Layer instance.
 * @param x Horizontal coordinate.
 * @param y Vertical coordinate.
 * @return The tile number located at the given coordinates or -1 if no tile is set.
 */
int MELLayerTileAtXAndY(MELLayer self, int x, int y);

/**
 * Changes the size of the layer to the given size.
 *
 * @param self Layer to resize.
 * @param newSize New size of the layer.
 */
void MELLayerResize(MELLayer * _Nonnull self, MELIntSize newSize);

void MELLayerRendererToSurfaceArray(MELLayer self, MELSurfaceArray * _Nonnull destination, MELTextureAtlas textureAtlas);

#endif /* layer_h */
