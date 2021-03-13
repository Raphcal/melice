//
//  renderer.c
//  shmup
//
//  Created by Raphaël Calabro on 09/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "renderer.h"

#include <assert.h>

typedef enum {
    MELDrawModeUnset,
    MELDrawModeTexture,
    MELDrawModeColor,
    MELDrawModeTextureAndColor
} MELDrawMode;

MELPoint MELRendererLastTranslation = {0, 0};
MELTexture *MELRendererLastTexture = NULL;
MELDrawMode MELRendererDrawMode = MELDrawModeUnset;
MELRectangle MELRendererFrame = {{-1, -1}, {2, 2}};

void MELRendererClearState(void) {
    MELRendererLastTranslation = MELPointMake(0, 0);
    MELRendererLastTexture = NULL;
    MELRendererDrawMode = MELDrawModeUnset;
}

void MELRendererInit(void) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glAlphaFunc(GL_GREATER, (GLclampf)0.1);
    
    glEnableClientState(GL_VERTEX_ARRAY);
}

void MELRendererApplyFlatOrthographicProjection(MELSize size) {
    MELRendererFrame = MELRectangleMake(0, 0, size.width, size.height);
    glLoadIdentity();
    glOrtho(0, size.width, 0, size.height, -1, 1);
    glTranslatef(0, size.height, 0);
}

MELRectangle MELRendererGetFrame(void) {
    return MELRendererFrame;
}

void MELRendererBindTexture(MELTexture * _Nonnull texture) {
    assert(texture != NULL);
    if (texture != MELRendererLastTexture) {
        MELRendererLastTexture = texture;
        glBindTexture(GL_TEXTURE_2D, texture->name);
    }
}

void MELRendererDrawWithVertexPointerAndTexCoordPointer(int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByTexture, GLfloat * _Nonnull texCoordPointer, GLsizei count) {
    if (MELRendererDrawMode != MELDrawModeTexture) {
        glDisableClientState(GL_COLOR_ARRAY);
        if (MELRendererDrawMode != MELDrawModeTextureAndColor) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnable(GL_TEXTURE_2D);
        }
        MELRendererDrawMode = MELDrawModeTexture;
    }
    glVertexPointer(coordinatesByVertex, GL_FLOAT, 0, vertexPointer);
    glTexCoordPointer(coordinatesByTexture, GL_FLOAT, 0, texCoordPointer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
}

void MELRendererDrawWithVertexPointerAndColorPointer(int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByColor, GLubyte * _Nonnull colorPointer, GLsizei count) {
    if (MELRendererDrawMode != MELDrawModeColor) {
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        if (MELRendererDrawMode != MELDrawModeTextureAndColor) {
            glEnableClientState(GL_COLOR_ARRAY);
        }
        MELRendererDrawMode = MELDrawModeColor;
    }
    
    glVertexPointer(coordinatesByVertex, GL_FLOAT, 0, vertexPointer);
    glColorPointer(coordinatesByColor, GL_UNSIGNED_BYTE, 0, colorPointer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
}

void MELRendererDrawWithVertexPointerTexCoordPointerAndColorPointer(int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByTexture, GLfloat * _Nonnull texCoordPointer, int coordinatesByColor, GLubyte * _Nonnull colorPointer, GLsizei count) {
    if (MELRendererDrawMode != MELDrawModeTextureAndColor) {
        if (MELRendererDrawMode == MELDrawModeTexture) {
            glEnableClientState(GL_COLOR_ARRAY);
        } else if (MELRendererDrawMode == MELDrawModeColor) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnable(GL_TEXTURE_2D);
        } else {
            glEnableClientState(GL_COLOR_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnable(GL_TEXTURE_2D);
        }
        MELRendererDrawMode = MELDrawModeTextureAndColor;
    }
    glVertexPointer(coordinatesByVertex, GL_FLOAT, 0, vertexPointer);
    glTexCoordPointer(coordinatesByTexture, GL_FLOAT, 0, texCoordPointer);
    glColorPointer(coordinatesByColor, GL_UNSIGNED_BYTE, 0, colorPointer);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
}

void MELRendererDrawWithSurfaceArray(MELSurfaceArray surfaceArray) {
    if (MELRendererDrawMode != MELDrawModeTexture) {
        glDisableClientState(GL_COLOR_ARRAY);
        if (MELRendererDrawMode != MELDrawModeTextureAndColor) {
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnable(GL_TEXTURE_2D);
        }
        MELRendererDrawMode = MELDrawModeTexture;
    }
    
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
    MELPoint translation = MELPointSubstract(topLeft, MELRendererLastTranslation);
    if (!MELPointEquals(translation, MELPointZero)) {
        glTranslatef(-translation.x, translation.y, 0);
        MELRendererLastTranslation = topLeft;
    }
}
