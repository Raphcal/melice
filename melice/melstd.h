//
//  melstd.h
//  melice
//
//  Created by Raphaël Calabro on 13/03/2021.
//

#ifndef melstd_h
#define melstd_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef __MACH__
#define GL_SILENCE_DEPRECATION 1
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE && !TARGET_OS_MACCATALYST
#import <OpenGLES/ES1/gl.h>
#define glOrtho glOrthof
#else
#include <OpenGL/gl.h>
#endif
#endif

#ifdef __linux__
#include <GL/gl.h>
#endif

#ifdef _WIN32
#include <windows.h>
#include <gl/GL.h>
#endif

#ifdef _WIN32
#define MELPathSeparator '\\'
#define MELPathSeparatorString "\\"
#else
#define MELPathSeparator '/'
#define MELPathSeparatorString "/"
#endif

#ifndef __clang__
#define _Nonnull
#define _Nullable
#endif

/**
 * Interval of time in seconds.
 */
typedef float MELTimeInterval;

/**
 * Boolean type.
 */
#ifndef __bool_true_false_are_defined
typedef enum {
    false,
    true
} MELBoolean;
#else
typedef bool MELBoolean;
#endif

#endif /* melstd_h */
