//
//  color.h
//  shmup
//
//  Created by Raphaël Calabro on 31/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef color_h
#define color_h

#include "melstd.h"

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

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} MELUInt8RGBColor;

/**
 * Color type.
 */
typedef uint32_t MELUInt32Color;

#define MELColorMake(red, green, blue, alpha) ((MELColor){red, green, blue, alpha})
MELColor MELColorMakeWithHex(uint32_t hexColor);
MELColor MELColorMakeWithMELUInt8Color(MELUInt8Color color);
MELUInt32Color MELColorToBGRAUInt32Color(MELColor self);
MELUInt32Color MELColorToRGBAUInt32Color(MELColor self);
MELUInt8Color MELColorToMELUInt8Color(MELColor self);

#define MELUInt8ColorMake(red, green, blue, alpha) ((MELUInt8Color){red, green, blue, alpha})
MELUInt8Color MELUInt8ColorMakeWithHex(uint32_t hexColor);
MELUInt8Color MELUInt8ColorBlendWithColor(MELUInt8Color self, MELUInt8Color incoming);

#define MELColorToUInt32Color MELColorToBGRAUInt32Color
MELUInt32Color MELUInt8ColorToRGBAUInt32Color(MELUInt8Color self);
MELUInt8Color MELRGBAUInt32ColorToMELUInt8Color(MELUInt32Color self);

#endif /* color_h */
