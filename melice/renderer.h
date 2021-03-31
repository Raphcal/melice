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

/**
 * Clear the renderer state.
 */
void MELRendererClearState(void);

/**
 * Initialize the renderer.
 */
void MELRendererInit(void);

/**
 * Apply a 2D orthographic projection of the given size.
 *
 * @param size Size of the screen after the projection has been applied.
 */
void MELRendererApplyFlatOrthographicProjection(MELSize size);

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
 * @param coordinatesByVertex Number of coordinates for each vertex.
 * @param vertexPointer Vertexes of the triangles to draw.
 * @param coordinatesByTexture Number of coordinates for each texture.
 * @param texCoordPointer Texture to apply to the triangles.
 * @param coordinatesByColor Number of coordinates for each color.
 * @param colorPointer Tint to apply to the given texture.
 * @param count Number of triangles to draw.
 */
void MELRendererDrawWithVertexPointerTexCoordPointerAndColorPointer(int coordinatesByVertex, GLfloat * _Nonnull vertexPointer, int coordinatesByTexture, GLfloat * _Nonnull texCoordPointer, int coordinatesByColor, GLubyte * _Nonnull colorPointer, GLsizei count);

void MELRendererDrawWithSurfaceArray(MELSurfaceArray surfaceArray);

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

#endif /* renderer_h */
