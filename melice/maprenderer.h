//
//  grid.h
//  shmup
//
//  Created by Raphaël Calabro on 30/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef maprenderer_h
#define maprenderer_h

#include "melstd.h"

#include "map.h"
#include "textureatlas.h"
#include "surfacearray.h"
#include "point.h"

#define MELTileSize 32

typedef struct {
    MELMap map;
    MELTextureAtlas atlas;
    MELSurfaceArray * _Nullable layerSurfaces;
} MELMapRenderer;

/**
 * Returns a map renderer with the given map.
 * Surfaces are allocated for this renderer. This is probably not what you want if you need to display sprites relatives to a layer.
 *
 * @param map Map to display.
 * @return A renderer.
 */
MELMapRenderer MELMapRendererMakeWithMapAndAtlas(MELMap map, MELTextureAtlas atlas);

MELMapRenderer MELMapRendererMakeWithMapAndPalette(MELMap map);

/**
 * Deinitialize and free the resources used by the given map renderer.
 * The map renderer itself is not freed.
 *
 * @param self Map renderer instance.
 */
void MELMapRendererDeinit(MELMapRenderer * _Nonnull self);

/**
 * Draw the entire map.
 *
 * @param self Map renderer instance.
 */
void MELMapRendererDraw(MELMapRenderer self);

/**
 * Apply the given translation and draw the entire map.
 *
 * @param self Map renderer instance.
 * @param translation Translation to apply.
 */
void MELMapRendererDrawTranslated(MELMapRenderer self, MELPoint translation);

/**
 * Apply the given translation and draw the layers in the given range.
 *
 * @param self Map renderer instance.
 * @param translation Translation to apply.
 * @param fromLayer Index of the first layer to draw.
 * @param toLayer Index of the last layer to draw.
 */
void MELMapRendererDrawRangeTranslated(MELMapRenderer self, MELPoint translation, unsigned int fromLayer, unsigned int toLayer);

void MELMapRendererDrawColorRangeTranslated(MELMapRenderer self, MELPoint translation, unsigned int fromLayer, unsigned int toLayer);

#endif /* maprenderer_h */
