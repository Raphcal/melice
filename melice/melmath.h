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

#define MEL_E    2.71828182845904523536028747135266250f  // e
#define MEL_PI   3.14159265358979323846264338327950288f  // π
#define MEL_PI_2 1.57079632679489661923132169163975144f  // π / 2
#define MEL_PI_4 0.785398163397448309615660845819875721f // π / 4

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
 * Returns the given value if it is greater than min and less than max.
 * Otherwise, min will be returned if the value is less than min or else max.
 *
 * @param min Minimum value.
 * @param value Value to test.
 * @param max Maximum value.
 * @return The value or one of the bounds.
 */
int32_t MELIntBound(int32_t min, int32_t value, int32_t max);

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

GLfloat MELEaseInOut(GLfloat from, GLfloat to, GLfloat value);

#endif /* melmath_h */
