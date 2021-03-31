//
//  rectangle.h
//  shmup
//
//  Created by Raphaël Calabro on 24/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef rectangle_h
#define rectangle_h

#include "melstd.h"

#include "point.h"
#include "size.h"

typedef struct melquadrilateral MELQuadrilateral;

/**
 * A rectangle.
 */
typedef struct melrectangle {
    MELPoint origin;
    MELSize size;
} MELRectangle;

/**
 * A rectangle with integer coordinates.
 */
typedef struct {
    MELIntPoint origin;
    MELIntSize size;
} MELIntRectangle;

/**
 * A rectangle whose origin and size are both zero.
 */
extern const MELRectangle MELRectangleZero;

/**
 * A rectangle whose origin and size are both zero.
 */
extern const MELIntRectangle MELIntRectangleZero;

/**
 * Returns a rectangle with the given origin and size.
 *
 * @param x Horizontal coordinate.
 * @param y Vertical coordinate.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @return A rectangle.
 */
MELRectangle MELRectangleMake(GLfloat x, GLfloat y, GLfloat width, GLfloat height);

/**
 * Returns a rectangle with the given origin and size.
 *
 * @param origin Rectangle origin. Maybe the center or the top left point depending on the context.
 * @param size Rectangle size.
 * @return A rectangle.
 */
MELRectangle MELRectangleMakeWithOriginAndSize(MELPoint origin, MELSize size);

/**
 * Returns a rectangle with the given origin and size.
 *
 * @param x Horizontal coordinate.
 * @param y Vertical coordinate.
 * @param width Width of the rectangle.
 * @param height Height of the rectangle.
 * @return A rectangle.
 */
MELIntRectangle MELIntRectangleMake(int32_t x, int32_t y, int32_t width, int32_t height);

/**
 * Returns a rectangle with the given origin and size.
 *
 * @param origin Rectangle origin. Maybe the center or the top left point depending on the context.
 * @param size Rectangle size.
 * @return A rectangle.
 */
MELIntRectangle MELIntRectangleMakeWithOriginAndSize(MELIntPoint origin, MELIntSize size);

/**
 * Returns the minimum X coordinate of a rectangle whose center is its origin.
 *
 * @param self Rectangle.
 * @return The left coordinate of the given rectangle.
 */
GLfloat MELRectangleOriginIsCenterGetLeft(MELRectangle self);

/**
 * Returns the maximum X coordinate of a rectangle whose center is its origin.
 *
 * @param self Rectangle.
 * @return The right coordinate of the given rectangle.
 */
GLfloat MELRectangleOriginIsCenterGetRight(MELRectangle self);

/**
 * Returns the minimum Y coordinate of a rectangle whose center is its origin.
 *
 * @param self Rectangle.
 * @return The top coordinate of the given rectangle.
 */
GLfloat MELRectangleOriginIsCenterGetTop(MELRectangle self);

/**
 * Returns the maximum Y coordinate of a rectangle whose center is its origin.
 *
 * @param self Rectangle.
 * @return The bottom coordinate of the given rectangle.
 */
GLfloat MELRectangleOriginIsCenterGetBottom(MELRectangle self);

MELBoolean MELIntRectangleEquals(MELIntRectangle lhs, MELIntRectangle rhs);

MELQuadrilateral MELRectangleRotate(MELRectangle self, GLfloat rotation);

MELQuadrilateral MELRectangleRotateWithPivot(MELRectangle self, GLfloat rotation, MELPoint pivot);

#endif /* rectangle_h */
