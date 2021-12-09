//
//  surface.c
//  shmup
//
//  Created by Raphaël Calabro on 24/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "surface.h"

#include <assert.h>
#include <string.h>
#include "surfacearray.h"
#include "surfacememory.h"

MELListImplement(MELSurface);

MELSurface MELSurfaceMake(MELSurfaceArray * _Nonnull parent, size_t vertex, size_t texture, size_t color) {
    return (MELSurface) {
        parent,
        vertex,
        texture,
        color
    };
}

MELSurface MELSurfaceMakeWithQuadIndex(MELSurfaceArray * _Nonnull parent, size_t quadIndex) {
    return (MELSurface) {
        parent,
        quadIndex * MELVertexesByQuad * MELCoordinatesByVertex,
        quadIndex * MELVertexesByQuad * MELCoordinatesByTexture,
        quadIndex * MELVertexesByQuad * MELCoordinatesByColor
    };
}

void MELSurfaceSetVerticesWithRectangle(MELSurface self, MELRectangle vertices) {
    MELSurfaceMemoryAppendRectangle(self.parent->vertex.memory + self.vertex, MELRectangleMake(vertices.origin.x, -vertices.origin.y, vertices.size.width, -vertices.size.height));
}

void MELSurfaceSetVerticesWithQuadrilateral(MELSurface self, MELQuadrilateral quadrilateral) {
    MELSurfaceMemoryAppendQuadrilateral(self.parent->vertex.memory + self.vertex, quadrilateral);
}

void MELSurfaceSetTexture(MELSurface self, int tile, MELTextureAtlas atlas) {
    assert(tile >= 0 && tile < atlas.frameCount);
    MELSurfaceMemoryAppendRectangle(self.parent->texture.memory + self.texture, atlas.frames[tile]);
}

void MELSurfaceSetColor(MELSurface self, MELUInt8Color color) {
    GLubyte *colorPointer = self.parent->color.memory + self.color;
    for (unsigned int index = 0; index < MELVertexesByQuad; index++) {
        MELSurfaceMemoryAppendColor(colorPointer, color);
        colorPointer += MELCoordinatesByColor;
    }
}

void MELSurfaceClear(MELSurface self) {
    MELSurfaceArray *parent = self.parent;
    memset(parent->vertex.memory + self.vertex, 0, sizeof(GLfloat) * MELVertexesByQuad * MELCoordinatesByVertex);
    memset(parent->texture.memory + self.texture, 0, sizeof(GLfloat) * MELVertexesByQuad * MELCoordinatesByTexture);
    memset(parent->color.memory + self.color, 0, sizeof(GLubyte) * MELVertexesByQuad * MELCoordinatesByColor);
}
