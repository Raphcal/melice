//
//  color.h
//  shmup
//
//  Created by Raphaël Calabro on 31/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef color_h
#define color_h

#include "../melstd.h"

/**
 * A color.
 */
typedef struct {
    GLfloat red;
    GLfloat green;
    GLfloat blue;
    GLfloat alpha;
} MELColor;

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} MELUInt8Color;

/**
 * Color type.
 */
typedef uint32_t MELUInt32Color;

#define MELColorMake(red, green, blue, alpha) ((MELColor){red, green, blue, alpha})

#define MELUInt8ColorMake(red, green, blue, alpha) ((MELUInt8Color){red, green, blue, alpha})

MELUInt32Color MELColorToUInt32Color(MELColor self);

#endif /* color_h */
