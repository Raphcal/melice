//
//  surfacearray.c
//  shmup
//
//  Created by Raphaël Calabro on 24/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "surfacearray.h"

#include <string.h>
#include <assert.h>
#include "surfacememory.h"

MELSurfaceArray MELSurfaceArrayMake(void) {
    return MELSurfaceArrayMakeWithInitialCapacity(100);
}

MELSurfaceArray MELSurfaceArrayMakeWithInitialCapacity(unsigned int initialCapacity) {
    return (MELSurfaceArray) {
        MELListMakeWithInitialCapacity(GLfloat, initialCapacity),
        MELListMakeWithInitialCapacity(GLfloat, initialCapacity),
        MELListMakeWithInitialCapacity(GLubyte, initialCapacity),
        0,
        MELListMake(MELSurface)
    };
}

void MELSurfaceArrayDeinit(MELSurfaceArray * _Nonnull self) {
    MELListDeinit(self->vertex);
    MELListDeinit(self->texture);
    MELListDeinit(self->color);
    self->count = 0;
    MELListDeinit(self->pool);
}

void MELListAppendRectangle(MELList(GLfloat) * _Nonnull self, MELRectangle rectangle) {
    const size_t cursor = self->count;
    MELListRefEnsureCapacity(self, cursor + 12);
    MELSurfaceMemoryAppendRectangle(self->memory + cursor, rectangle);
    self->count = cursor + 12;
}

void MELListAppendColor(MELList(GLubyte) * _Nonnull self, MELUInt8Color color) {
    const size_t cursor = self->count;
    MELListRefEnsureCapacity(self, cursor + 4);
    MELSurfaceMemoryAppendColor(self->memory + cursor, color);
    self->count = cursor + 4;
}

void MELSurfaceArrayAppendTexturedQuad(MELSurfaceArray * _Nonnull self, MELRectangle vertices, int tile, MELTextureAtlas atlas) {
    assert(tile >= 0 && tile < atlas.frameCount);
    self->count += MELVertexesByQuad;
    MELListAppendRectangle(&self->vertex, MELRectangleMake(vertices.origin.x, -vertices.origin.y, vertices.size.width, -vertices.size.height));
    MELListAppendRectangle(&self->texture, atlas.frames[tile]);
}

void MELSurfaceArrayAppendColoredQuad(MELSurfaceArray * _Nonnull self, MELRectangle vertices, MELUInt8Color color) {
    self->count += MELVertexesByQuad;
    MELListAppendRectangle(&self->vertex, MELRectangleMake(vertices.origin.x, -vertices.origin.y, vertices.size.width, -vertices.size.height));
    MELList(GLubyte) *colors = &self->color;
    MELListAppendColor(colors, color);
    MELListAppendColor(colors, color);
    MELListAppendColor(colors, color);
    MELListAppendColor(colors, color);
    MELListAppendColor(colors, color);
    MELListAppendColor(colors, color);
}

void MELSurfaceArrayClear(MELSurfaceArray * _Nonnull self) {
    self->count = 0;
    self->vertex.count = 0;
    self->texture.count = 0;
    self->color.count = 0;
    self->pool.count = 0;
}

MELSurface MELSurfaceArrayAvailableSurface(MELSurfaceArray * _Nonnull self) {
    if (self->pool.count > 0) {
        return MELListPop(self->pool);
    }
    // TODO: Optimiser le code
    MELList(GLfloat) *vertex = &self->vertex;
    MELList(GLfloat) *texture = &self->texture;
    MELList(GLubyte) *color = &self->color;
    
    MELSurface surface = MELSurfaceMake(self, vertex->count, texture->count, color->count);
    vertex->count += MELCoordinatesByVertex * MELVertexesByQuad;
    texture->count += MELCoordinatesByTexture * MELVertexesByQuad;
    color->count += MELCoordinatesByColor * MELVertexesByQuad;
    MELListRefEnsureCapacity(vertex, vertex->count);
    MELListRefEnsureCapacity(texture, texture->count);
    MELListRefEnsureCapacity(color, color->count);
    self->count += MELVertexesByQuad;
    return surface;
}

void MELSurfaceArrayReleaseSurface(MELSurfaceArray * _Nonnull self, MELSurface surface) {
    MELSurfaceClear(surface);
    MELListPush(self->pool, surface);
}
