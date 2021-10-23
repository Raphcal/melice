//
//  renderer.h
//  shmup
//
//  Created by Raphaël Calabro on 09/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef renderer_h
#define renderer_h

#include "melstd.h"

#include "color.h"
#include "texture.h"
#include "surfacearray.h"
#include "point.h"
#include "rectangle.h"
#include "size.h"

typedef enum {
    MELDrawModeUnset,
    MELDrawModeTexture,
    MELDrawModeColor,
    MELDrawModeTextureAndColor
} MELDrawMode;

typedef struct {
    MELPoint lastTranslation;
    MELTexture * _Nullable lastTexture;
    MELDrawMode drawMode;
    MELRectangle frame;
} MELRenderer;

/**
 * A renderer whose translation is zero, last texture is null, draw mode is unset and frame is zero.
 */
extern const MELRenderer MELRendererZero;

/**
 * Clear the renderer state.
 */
void MELRendererClearState(void);

/**
 * Clear the given renderer state.
 *
 * @param self Renderer instance.
 */
void MELRendererRefClearState(MELRenderer * _Nonnull self);

/**
 * Initialize the renderer.
 */
void MELRendererInit(void);

/**
 * Get the default renderer;
 */
MELRenderer * _Nonnull MELRendererGetDefault(void);

/**
 * Apply a 2D orthographic projection of the given size.
 *
 * @param size Size of the screen after the projection has been applied.
 */
void MELRendererApplyFlatOrthographicProjection(MELSize size);

/**
 * Apply a 2D orthographic projection of the given size and save frame to the given renderer.
 *
 * @param self Renderer instance.
 * @param size Size of the screen after the projection has been applied.
 */
void MELRendererRefApplyFlatOrthographicProjection(MELRenderer * _Nonnull self, MELSize size);

/**
 * Returns the current size and top left position of the view.
 *
 * @return A rectangle defining the size and top left position of the view.
 */
MELRectangle MELRendererGetFrame(void);

/**
 * Binds the given texture.
 *
 * @param texture Texture to bind.
 */
void MELRendererBindTexture(MELTexture * _Nonnull texture);

/**
 * Binds the given texture to the given renderer.
 *
 * @param self Renderer instance.
 * @param texture Texture to bind.
 */
void MELRendererRefBindTexture(MELRenderer * _Nonnull self, MELTexture * _Nonnull texture);

/**
 * Draw the given surfaces.
 *
 * @param coordinatesByVertex Number of coordinates for each vertex.
 * @param vertexPointer Vertexes of the triangles to draw.
 * @param coordinatesByTexture Number of coordinates for each texture.
 * @param texCoordPointer Texture to apply to the triangles.
 * @param count Number of triangles to draw.
 */
void MELRendererDrawWithVertexPointerAndTexCoordPointer(int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByTexture, GLfloat * _Nonnull texCoordPointer, GLsizei count);

/**
 * Draw the given surfaces.
 *
 * @param self Renderer instance.
 * @param coordinatesByVertex Number of coordinates for each vertex.
 * @param vertexPointer Vertexes of the triangles to draw.
 * @param coordinatesByTexture Number of coordinates for each texture.
 * @param texCoordPointer Texture to apply to the triangles.
 * @param count Number of triangles to draw.
 */
void MELRendererRefDrawWithVertexPointerAndTexCoordPointer(MELRenderer * _Nonnull self, int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByTexture, GLfloat * _Nonnull texCoordPointer, GLsizei count);

/**
 * Draw the given surfaces.
 *
 * @param coordinatesByVertex Number of coordinates for each vertex.
 * @param vertexPointer Vertexes of the triangles to draw.
 * @param coordinatesByColor Number of coordinates for each color.
 * @param colorPointer Color to apply to the triangles.
 * @param count Number of triangles to draw.
 */
void MELRendererDrawWithVertexPointerAndColorPointer(int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByColor, GLubyte * _Nonnull colorPointer, GLsizei count);

/**
 * Draw the given surfaces.
 *
 * @param self Renderer instance.
 * @param coordinatesByVertex Number of coordinates for each vertex.
 * @param vertexPointer Vertexes of the triangles to draw.
 * @param coordinatesByColor Number of coordinates for each color.
 * @param colorPointer Color to apply to the triangles.
 * @param count Number of triangles to draw.
 */
void MELRendererRefDrawWithVertexPointerAndColorPointer(MELRenderer * _Nonnull self, int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByColor, GLubyte * _Nonnull colorPointer, GLsizei count);

/**
 * Draw the given surfaces.
 *
 * @param coordinatesByVertex Number of coordinates for each vertex.
 * @param vertexPointer Vertexes of the triangles to draw.
 * @param coordinatesByTexture Number of coordinates for each texture.
 * @param texCoordPointer Texture to apply to the triangles.
 * @param coordinatesByColor Number of coordinates for each color.
 * @param colorPointer Tint to apply to the given texture.
 * @param count Number of triangles to draw.
 */
void MELRendererDrawWithVertexPointerTexCoordPointerAndColorPointer(int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByTexture, GLfloat * _Nonnull texCoordPointer, int coordinatesByColor, GLubyte * _Nonnull colorPointer, GLsizei count);

/**
 * Draw the given surfaces.
 *
 * @param self Renderer instance.
 * @param coordinatesByVertex Number of coordinates for each vertex.
 * @param vertexPointer Vertexes of the triangles to draw.
 * @param coordinatesByTexture Number of coordinates for each texture.
 * @param texCoordPointer Texture to apply to the triangles.
 * @param coordinatesByColor Number of coordinates for each color.
 * @param colorPointer Tint to apply to the given texture.
 * @param count Number of triangles to draw.
 */
void MELRendererRefDrawWithVertexPointerTexCoordPointerAndColorPointer(MELRenderer * _Nonnull self, int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByTexture, GLfloat * _Nonnull texCoordPointer, int coordinatesByColor, GLubyte * _Nonnull colorPointer, GLsizei count);

/**
 * Draw the vertices using the given surface array.
 *
 * @param surfaceArray Surfaces to draw.
 */
void MELRendererDrawWithSurfaceArray(MELSurfaceArray surfaceArray);

/**
 * Draw the vertices using the given surface array.
 *
 * @param self Renderer instance.
 * @param surfaceArray Surfaces to draw.
 */
void MELRendererRefDrawWithSurfaceArray(MELRenderer * _Nonnull self, MELSurfaceArray surfaceArray);

/**
 * Clear the screen with the given color.
 *
 * @param color Color to use to clear the screen.
 */
void MELRendererClearWithColor(MELColor color);

/**
 * Translate the drawing context to the given top left point.
 *
 * @param topLeft Coordinates to translate to.
 */
void MELRendererTranslateToTopLeft(MELPoint topLeft);

/**
 * Translate the drawing context to the given top left point.
 *
 * @param self Renderer instance.
 * @param topLeft Coordinates to translate to.
 */
void MELRendererRefTranslateToTopLeft(MELRenderer * _Nonnull self, MELPoint topLeft);

void MELRendererRefSetDrawMode(MELRenderer * _Nonnull self, MELDrawMode drawMode);

#endif /* renderer_h */
