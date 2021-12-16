//
//  renderer.c
//  shmup
//
//  Created by Raphaël Calabro on 09/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "renderer.h"

#include <assert.h>

const MELRenderer MELRendererZero = {{0, 0}, NULL, MELDrawModeUnset, {{-1, -1}, {2, 2}}};

MELRenderer defaultRenderer = {{0, 0}, NULL, MELDrawModeUnset, {{-1, -1}, {2, 2}}};

void MELRendererClearState(void) {
    defaultRenderer.lastTranslation = MELPointMake(0, 0);
    defaultRenderer.lastTexture = NULL;
    defaultRenderer.drawMode = MELDrawModeUnset;
}

void MELRendererInit(void) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, (GLclampf)0.1);

    glEnableClientState(GL_VERTEX_ARRAY);
}

MELRenderer * _Nonnull MELRendererGetDefault(void) {
    return &defaultRenderer;
}

void MELRendererApplyFlatOrthographicProjection(MELSize size) {
    MELRendererRefApplyFlatOrthographicProjection(&defaultRenderer, size);
}

void MELRendererRefApplyFlatOrthographicProjection(MELRenderer * _Nonnull self, MELSize size) {
    self->frame = MELRectangleMake(0, 0, size.width, size.height);
    self->lastTranslation = MELPointZero;
    glLoadIdentity();
    glOrtho(0, size.width, 0, size.height, -1, 1);
    glTranslatef(0, size.height, 0);
}

MELRectangle MELRendererGetFrame(void) {
    return defaultRenderer.frame;
}

void MELRendererBindTexture(MELTexture * _Nonnull texture) {
    MELRendererRefBindTexture(&defaultRenderer, texture);
}

void MELRendererRefBindTexture(MELRenderer * _Nonnull self, MELTexture * _Nonnull texture) {
    assert(texture != NULL);
    if (texture != self->lastTexture) {
        self->lastTexture = texture;
        glBindTexture(GL_TEXTURE_2D, texture->name);
    }
}

void MELRendererDrawWithVertexPointerAndTexCoordPointer(int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByTexture, GLfloat * _Nonnull texCoordPointer, GLsizei count) {
    MELRendererRefDrawWithVertexPointerAndTexCoordPointer(&defaultRenderer, coordinatesByVertex, vertexPointer, coordinatesByTexture, texCoordPointer, count);
}

void MELRendererRefDrawWithVertexPointerAndTexCoordPointer(MELRenderer * _Nonnull self, int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByTexture, GLfloat * _Nonnull texCoordPointer, GLsizei count) {
    MELRendererRefSetDrawMode(self, MELDrawModeTexture);
    glVertexPointer(coordinatesByVertex, GL_FLOAT, 0, vertexPointer);
    glTexCoordPointer(coordinatesByTexture, GL_FLOAT, 0, texCoordPointer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
}


void MELRendererRefDrawWithVertexPointerAndTexCoordPointerAndIndexPointer(MELRenderer * _Nonnull self, GLfloat * _Nonnull vertexPointer, GLfloat * _Nonnull texCoordPointer, GLint * _Nonnull indexPointer, GLsizei count) {
    MELRendererRefSetDrawMode(self, MELDrawModeTexture);
    glEnableClientState(GL_INDEX_ARRAY);
    glVertexPointer(MELCoordinatesByVertex, GL_FLOAT, 0, vertexPointer);
    glTexCoordPointer(MELCoordinatesByTexture, GL_FLOAT, 0, texCoordPointer);
    glIndexPointer(GL_INT, 0, indexPointer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
}

void MELRendererDrawWithVertexPointerAndColorPointer(int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByColor, GLubyte * _Nonnull colorPointer, GLsizei count) {
    MELRendererRefDrawWithVertexPointerAndColorPointer(&defaultRenderer, coordinatesByVertex, vertexPointer, coordinatesByColor, colorPointer, count);
}

void MELRendererRefDrawWithVertexPointerAndColorPointer(MELRenderer * _Nonnull self, int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByColor, GLubyte * _Nonnull colorPointer, GLsizei count) {
    MELRendererRefSetDrawMode(self, MELDrawModeColor);
    glVertexPointer(coordinatesByVertex, GL_FLOAT, 0, vertexPointer);
    glColorPointer(coordinatesByColor, GL_UNSIGNED_BYTE, 0, colorPointer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
}

void MELRendererDrawWithVertexPointerTexCoordPointerAndColorPointer(int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByTexture, GLfloat * _Nonnull texCoordPointer, int coordinatesByColor, GLubyte * _Nonnull colorPointer, GLsizei count) {
    MELRendererRefDrawWithVertexPointerTexCoordPointerAndColorPointer(&defaultRenderer, coordinatesByVertex, vertexPointer, coordinatesByTexture, texCoordPointer, coordinatesByColor, colorPointer, count);
}

void MELRendererRefDrawWithVertexPointerTexCoordPointerAndColorPointer(MELRenderer * _Nonnull self, int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByTexture, GLfloat * _Nonnull texCoordPointer, int coordinatesByColor, GLubyte * _Nonnull colorPointer, GLsizei count) {
    MELRendererRefSetDrawMode(self, MELDrawModeTextureAndColor);
    glVertexPointer(coordinatesByVertex, GL_FLOAT, 0, vertexPointer);
    glTexCoordPointer(coordinatesByTexture, GL_FLOAT, 0, texCoordPointer);
    glColorPointer(coordinatesByColor, GL_UNSIGNED_BYTE, 0, colorPointer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
}

void MELRendererDrawWithSurfaceArray(MELSurfaceArray surfaceArray) {
    MELRendererRefDrawWithSurfaceArray(&defaultRenderer, surfaceArray);
}

void MELRendererRefDrawWithSurfaceArray(MELRenderer * _Nonnull self, MELSurfaceArray surfaceArray) {
    MELRendererRefSetDrawMode(self, MELDrawModeTexture);

    // TODO: Gérer l'affichage des couleurs. Ajouter un mode ?
    glVertexPointer(MELCoordinatesByVertex, GL_FLOAT, 0, surfaceArray.vertex.memory);
    glTexCoordPointer(MELCoordinatesByTexture, GL_FLOAT, 0, surfaceArray.texture.memory);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, surfaceArray.count);
}

void MELRendererClearWithColor(MELColor color) {
    glClearColor(color.red, color.green, color.blue, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MELRendererTranslateToTopLeft(MELPoint topLeft) {
    MELRendererRefTranslateToTopLeft(&defaultRenderer, topLeft);
}

void MELRendererRefTranslateToTopLeft(MELRenderer * _Nonnull self, MELPoint topLeft) {
    MELPoint translation = MELPointSubstract(topLeft, self->lastTranslation);
    if (!MELPointEquals(translation, MELPointZero)) {
        glTranslatef(-translation.x, translation.y, 0);
        self->lastTranslation = topLeft;
    }
}

MELBoolean isDrawModeIncluded(MELDrawMode self, MELDrawMode other) {
    return self == other || self == MELDrawModeTextureAndColor;
}

void MELRendererRefSetDrawMode(MELRenderer * _Nonnull self, MELDrawMode drawMode) {
    if (self->drawMode == drawMode) {
        return;
    }

    const MELBoolean textureIsEnabled = isDrawModeIncluded(self->drawMode, MELDrawModeTexture);
    const MELBoolean wantTextureToBeEnabled = isDrawModeIncluded(drawMode, MELDrawModeTexture);
    if (textureIsEnabled && !wantTextureToBeEnabled) {
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    } else if (!textureIsEnabled && wantTextureToBeEnabled) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_TEXTURE_2D);
    }

    const MELBoolean colorIsEnabled = isDrawModeIncluded(self->drawMode, MELDrawModeColor);
    const MELBoolean wantColorToBeEnabled = isDrawModeIncluded(drawMode, MELDrawModeColor);
    if (colorIsEnabled && !wantColorToBeEnabled) {
        glDisableClientState(GL_COLOR_ARRAY);
    } else if (!colorIsEnabled && wantColorToBeEnabled) {
        glEnableClientState(GL_COLOR_ARRAY);
    }
    self->drawMode = drawMode;
}
