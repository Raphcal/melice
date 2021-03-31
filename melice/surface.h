//
//  surface.h
//  shmup
//
//  Created by Raphaël Calabro on 24/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef surface_h
#define surface_h

#include "melstd.h"

#include "color.h"
#include "textureatlas.h"
#include "rectangle.h"
#include "quadrilateral.h"
#include "list.h"

#define MELVertexesByQuad 6
#define MELCoordinatesByVertex 2
#define MELCoordinatesByTexture 2
#define MELCoordinatesByColor 4

typedef struct melsurfacearray MELSurfaceArray;

typedef struct {
    MELSurfaceArray * _Nonnull parent;
    size_t vertex;
    size_t texture;
    size_t color;
} MELSurface;

MELListDefine(MELSurface);

/**
 * Returns a surface with the given memory, number of coordinates and number of vertexes by quad.
 *
 * @param parent Parent surface array.
 * @param vertex Vertex index.
 * @param texture Texture index.
 * @param color Color index.
 * @return A surface.
 */
MELSurface MELSurfaceMake(MELSurfaceArray * _Nonnull parent, size_t vertex, size_t texture, size_t color);

void MELSurfaceSetVerticesWithRectangle(MELSurface self, MELRectangle vertices);

void MELSurfaceSetVerticesWithQuadrilateral(MELSurface self, MELQuadrilateral quadrilateral);

void MELSurfaceSetTexture(MELSurface self, int tile, MELTextureAtlas atlas);

void MELSurfaceSetColor(MELSurface self, int tile, MELUInt8Color color);

void MELSurfaceClear(MELSurface self);

#endif /* surface_h */
