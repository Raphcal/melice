//
//  math.c
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
    return value < min ? min : value < max ? value : max;
}

int64_t MELInt64Max(int64_t lhs, int64_t rhs) {
    return lhs > rhs ? lhs : rhs;
}

GLfloat MELDegreesToRadians(GLfloat degrees) {
    return degrees * M_PI / 180;
}
