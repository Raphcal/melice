//
//  surfacearray.h
//  shmup
//
//  Created by Raphaël Calabro on 24/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef surfacearray_h
#define surfacearray_h

#include "melstd.h"

#include "surface.h"
#include "textureatlas.h"
#include "rectangle.h"
#include "list.h"
#include "primitives.h"
#include "spriteinstance.h"

typedef struct melsurfacearray {
    MELList(GLfloat) vertex;
    MELList(GLfloat) texture;
    MELList(GLubyte) color;
    MELList(GLint) index;
    GLsizei count;
    MELList(MELSurface) pool;
} MELSurfaceArray;

MELListDefine(MELSurfaceArray);

/**
 * Returns a surface array with the given capacity.
 * Memory is allocated by this method. You have to deallocate it with `MELSurfaceArrayDeinit`.
 *
 * @return A surface array.
 */
MELSurfaceArray MELSurfaceArrayMake(void);

MELSurfaceArray MELSurfaceArrayMakeWithInitialCapacity(unsigned int initialCapacity);

/**
 * Deinit and free the resources allocated for the given surface array.
 * The surface array itself is not freed.
 *
 * @param self Surface array instance.
 */
void MELSurfaceArrayDeinit(MELSurfaceArray * _Nonnull self);

void MELSurfaceArrayAppendTexturedQuad(MELSurfaceArray * _Nonnull self, MELRectangle vertices, int tile, MELTextureAtlas atlas);

void MELSurfaceArrayAppendColoredQuad(MELSurfaceArray * _Nonnull self, MELRectangle vertices, MELUInt8Color color);

void MELSurfaceArrayAppendTexturedColoredQuad(MELSurfaceArray * _Nonnull self, MELRectangle vertices, int tile, MELTextureAtlas atlas, MELUInt8Color color);

void MELSurfaceArrayAppendTilesWithAlpha(MELSurfaceArray * _Nonnull self, int32_t * _Nonnull tiles, MELIntSize size, MELIntSize tileSize, uint8_t alpha, MELTextureAtlas atlas);

/**
 * Remove every surface but keep the allocated memory.
 */
void MELSurfaceArrayClear(MELSurfaceArray * _Nonnull self);

/**
 * Clear the pool and its allocated surfaces.
 */
void MELSurfaceArrayClearPool(MELSurfaceArray * _Nonnull self);

/**
 * Returns a surface from the pool if available, creates a new one otherwise.
 */
MELSurface MELSurfaceArrayAvailableSurface(MELSurfaceArray * _Nonnull self);

/**
 * Resets the given surface and inserts it in the pool.
 */
void MELSurfaceArrayReleaseSurface(MELSurfaceArray * _Nonnull self, MELSurface surface);

#endif /* surfacearray_h */
