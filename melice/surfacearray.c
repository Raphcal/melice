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

MELListImplement(MELSurfaceArray)

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

void MELListAppendTile(MELList(GLfloat) * _Nonnull self, MELRectangle rectangle) {
    const size_t cursor = self->count;
    GLfloatListEnsureCapacity(self, cursor + MELVertexesByTile * MELCoordinatesByVertex);
    MELSurfaceMemoryAppendTile(self->memory + cursor, rectangle);
    self->count = cursor + MELVertexesByTile * MELCoordinatesByVertex;
}

void MELListAppendSkip(MELList(GLfloat) * _Nonnull self, MELRectangle rectangle) {
    const size_t cursor = self->count;
    GLfloatListEnsureCapacity(self, cursor + MELVertexesBySkip * MELCoordinatesByVertex);
    MELSurfaceMemoryAppendSkip(self->memory + cursor, rectangle);
    self->count = cursor + MELVertexesBySkip * MELCoordinatesByVertex;
}

void MELListAppendJoin(MELList(GLfloat) * _Nonnull self, MELRectangle rectangle) {
    const size_t cursor = self->count;
    GLfloatListEnsureCapacity(self, cursor + MELVertexesByJoin * MELCoordinatesByVertex);
    MELSurfaceMemoryAppendJoin(self->memory + cursor, rectangle);
    self->count = cursor + MELVertexesByJoin * MELCoordinatesByVertex;
}

void MELListAppendLastVertex(MELList(GLfloat) * _Nonnull self) {
    const size_t cursor = self->count;
    GLfloatListEnsureCapacity(self, cursor + MELCoordinatesByVertex);
    self->memory[cursor] = self->memory[cursor - 2];
    self->memory[cursor + 1] = self->memory[cursor - 1];
    self->count = cursor + 2;
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

void MELSurfaceArrayAppendTexturedLayerTile(MELSurfaceArray * _Nonnull self, MELRectangle vertices, int tile, MELTextureAtlas atlas) {
    const MELRectangle vertRect = MELRectangleMake(vertices.origin.x, -vertices.origin.y, vertices.size.width, -vertices.size.height);

    const MELBoolean isTile = tile >= 0 && tile < atlas.frameCount;
    const MELBoolean previousWasSkip = self->count % 2 == 1;
    if (isTile && previousWasSkip) {
        self->count += MELVertexesByJoin + MELVertexesByTile;
        MELListAppendJoin(&self->vertex, vertRect);
        MELListAppendTile(&self->vertex, vertRect);
        MELListAppendJoin(&self->texture, atlas.frames[tile]);
        MELListAppendTile(&self->texture, atlas.frames[tile]);
    } else if (isTile) {
        self->count += MELVertexesByTile;
        MELListAppendTile(&self->vertex, vertRect);
        MELListAppendTile(&self->texture, atlas.frames[tile]);
    } else if (!previousWasSkip && self->count > 0) {
        self->count += MELVertexesBySkip;
        MELListAppendSkip(&self->vertex, vertRect);
        MELListAppendSkip(&self->texture, atlas.frames[tile]);
    }
}

void MELSurfaceArrayAppendSkip(MELSurfaceArray * _Nonnull self) {
    if (self->count == 0) {
        return;
    }
    MELListAppendLastVertex(&self->vertex);
    MELListAppendLastVertex(&self->texture);
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
