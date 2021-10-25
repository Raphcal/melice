//
//  melmath.h
//  shmup
//
//  Created by Raphaël Calabro on 11/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef melmath_h
#define melmath_h

#include "melstd.h"

/**
 * Returns the minimum value between lhs and rhs.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 * @return The minimum value between lhs and rhs.
 */
GLfloat MELFloatMin(GLfloat lhs, GLfloat rhs);

/**
 * Returns the minimum value between lhs and rhs.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 * @return The minimum value between lhs and rhs.
 */
uint8_t MELUInt8Min(int lhs, int rhs);

/**
 * Returns the minimum value among the given values.
 *
 * @param count Number of arguments.
 * @param ... Values.
 * @return The minimum value.
 */
double MELDoubleMinVariadic(int count, ...);

/**
 * Returns the maximum value between lhs and rhs.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 * @return The maximum value between lhs and rhs.
 */
GLfloat MELFloatMax(GLfloat lhs, GLfloat rhs);

/**
 * Returns the maximum value among the given values.
 *
 * @param count Number of arguments.
 * @param ... Values.
 * @return The maximum value.
 */
double MELDoubleMaxVariadic(int count, ...);

/**
 * Returns the given value if it is greater than min and less than max.
 * Otherwise, min will be returned if the value is less than min or else max.
 *
 * @param min Minimum value.
 * @param value Value to test.
 * @param max Maximum value.
 * @return The value or one of the bounds.
 */
GLfloat MELFloatBound(GLfloat min, GLfloat value, GLfloat max);

/**
 * Returns the maximum value between lhs and rhs.
 *
 * @param lhs Left operand.
 * @param rhs Right operand.
 * @return The maximum value between lhs and rhs.
 */
int64_t MELInt64Max(int64_t lhs, int64_t rhs);

/**
 * Converts and returns the given value to radians.
 *
 * @param degrees Angle in degrees.
 * @return The given angle in radians.
 */
GLfloat MELDegreesToRadians(GLfloat degrees);


#endif /* melmath_h */
