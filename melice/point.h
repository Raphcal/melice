//
//  point.h
//  shmup
//
//  Created by Raphaël Calabro on 24/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef point_h
#define point_h

#include "melstd.h"

typedef struct melsize MELSize;

/**
 * Floating coordinates of a point in a 2D plane.
 */
typedef struct {
    GLfloat x;
    GLfloat y;
} MELPoint;

/**
 * Integer coordinates of a point in a 2D plane.
 */
typedef struct {
    int32_t x;
    int32_t y;
} MELIntPoint;

/**
 * A point whose coordinates are both zero.
 */
extern const MELPoint MELPointZero;

/**
 * A point whose coordinates are both zero.
 */
extern const MELIntPoint MELIntPointZero;

/**
 * Returns a point with the given coordinates.
 *
 * @param x Horizontal coordinate.
 * @param y Vertical coordinate.
 * @return A point.
 */
MELPoint MELPointMake(GLfloat x, GLfloat y);

/**
 * Returns a point with the given coordinates.
 *
 * @param x Horizontal coordinate.
 * @param y Vertical coordinate.
 * @return A point.
 */
MELIntPoint MELIntPointMake(int32_t x, int32_t y);

/**
 * Compares the two given point.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 * @return true if the points are the same, false otherwise.
 */
MELBoolean MELPointEquals(MELPoint lhs, MELPoint rhs);

/**
 * Adds the given points.
 *
 * @return A point whose coordinates are lhs.x + rhs.x and lhs.y + rhs.y.
 */
MELPoint MELPointAdd(MELPoint lhs, MELPoint rhs);

/**
 * Adds the given points.
 *
 * @return A point whose coordinates are `point.x + size.width` and `point.y + size.height`.
 */
MELPoint MELPointAddSize(MELPoint point, MELSize size);

/**
 * Substracts the given points.
 *
 * @return A point whose coordinates are lhs.x - rhs.x and lhs.y - rhs.y.
 */
MELPoint MELPointSubstract(MELPoint lhs, MELPoint rhs);

/**
 * Substracts the given size to the given point.
 *
 * @return A point whose coordinates are `point.x - size.width` and `point.y - size.height`.
 */
MELPoint MELPointSubstractSize(MELPoint point, MELSize size);

/**
 * Multiplies the given points.
 *
 * @return A point whose coordinates are lhs.x * rhs.x and lhs.y * rhs.y.
 */
MELPoint MELPointMultiply(MELPoint lhs, MELPoint rhs);

/**
 * Multiplies the coordinates of the given points by the given value.
 *
 * @return A point whose coordinates are self.x * value and self.y * value.
 */
MELPoint MELPointMultiplyByValue(MELPoint self, GLfloat value);

/**
 * Returns the distance between the 2 points.
 *
 * @param self Source point.
 * @param other Other point.
 * @return The distance between the points.
 */
GLfloat MELPointDistanceToPoint(MELPoint self, MELPoint other);

/**
 * Returns the angle between the 2 points.
 *
 * @param self Source point.
 * @param other Other point.
 * @return The angle between the points.
 */
GLfloat MELPointAngleToPoint(MELPoint self, MELPoint other);

#endif /* point_h */
