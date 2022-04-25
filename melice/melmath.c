//
//  melmath.c
//  shmup
//
//  Created by Raphaël Calabro on 11/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "melmath.h"

#include <stdarg.h>

GLfloat MELFloatMin(GLfloat lhs, GLfloat rhs) {
    return lhs < rhs ? lhs : rhs;
}

uint8_t MELUInt8Min(int lhs, int rhs) {
    return (uint8_t) (lhs < rhs ? lhs : rhs);
}

double MELDoubleMinVariadic(int count, ...) {
    va_list list;
    va_start(list, count);
    double min = va_arg(list, double);
    for (int index = 1; index < count; index++) {
        min = MELFloatMin(min, va_arg(list, double));
    }
    va_end(list);
    return min;
}

GLfloat MELFloatMax(GLfloat lhs, GLfloat rhs) {
    return lhs > rhs ? lhs : rhs;
}

double MELDoubleMaxVariadic(int count, ...) {
    va_list list;
    va_start(list, count);
    double max = va_arg(list, double);
    for (int index = 1; index < count; index++) {
        max = MELFloatMax(max, va_arg(list, double));
    }
    va_end(list);
    return max;
}

GLfloat MELFloatBound(GLfloat min, GLfloat value, GLfloat max) {
    return min > max ? min : (value < min ? min : (value < max ? value : max));
}

int32_t MELIntBound(int32_t min, int32_t value, int32_t max) {
    return min > max ? min : (value < min ? min : (value < max ? value : max));
}

int64_t MELInt64Max(int64_t lhs, int64_t rhs) {
    return lhs > rhs ? lhs : rhs;
}

GLfloat MELDegreesToRadians(GLfloat degrees) {
    return degrees * M_PI / 180;
}

GLfloat MELDifferenceBetweenAngles(GLfloat lhs, GLfloat rhs) {
    const GLfloat difference = rhs - lhs;

    if (difference < -MEL_PI) {
        return difference + MEL_PI * 2;
    } else if (difference > MEL_PI) {
        return difference - MEL_PI * 2;
    } else {
        return difference;
    }
}

GLfloat MELEaseInOut(GLfloat from, GLfloat to, GLfloat value) {
    return powf(sinf(MEL_PI / 2 * fminf(fmaxf(value - from, 0) / (to - from), 1)), 2);
}
