//
//  color.c
//  shmup
//
//  Created by Raphaël Calabro on 31/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "color.h"

#include "melmath.h"

MELColor MELColorMakeWithHex(uint32_t hexColor) {
    return (MELColor) {
        ((hexColor & 0xFF0000) >> 16) / 255.0f,
        ((hexColor & 0xFF00) >> 8) / 255.0f,
        (hexColor & 0xFF) / 255.0f,
        1
    };
}

MELUInt8Color MELColorToMELUInt8Color(MELColor self) {
    return (MELUInt8Color) {
        (uint8_t) (self.red * 255.0f),
        (uint8_t) (self.green * 255.0f),
        (uint8_t) (self.blue * 255.0f),
        (uint8_t) (self.alpha * 255.0f)
    };
}

MELUInt32Color MELColorToBGRAUInt32Color(MELColor self) {
    return ((MELUInt32Color)(self.blue * 255)) + ((MELUInt32Color)(self.green * 255) << 8) + ((MELUInt32Color)(self.red * 255) << 16) + ((MELUInt32Color)(self.alpha * 255) << 24);
}

MELUInt32Color MELColorToRGBAUInt32Color(MELColor self) {
    return ((MELUInt32Color)(self.red * 255)) + ((MELUInt32Color)(self.green * 255) << 8) + ((MELUInt32Color)(self.blue * 255) << 16) + ((MELUInt32Color)(self.alpha * 255) << 24);
}

MELUInt8Color MELRGBAUInt32ColorToMELUInt8Color(MELUInt32Color self) {
    return *((MELUInt8Color *) &self);
}

MELUInt8Color MELUInt8ColorMakeWithHex(uint32_t hexColor) {
    return (MELUInt8Color) {
        (hexColor & 0xFF0000) >> 16,
        (hexColor & 0xFF00) >> 8,
        hexColor & 0xFF,
        255
    };
}

MELUInt32Color MELUInt8ColorToRGBAUInt32Color(MELUInt8Color self) {
    return *((MELUInt32Color *) &self);
}


MELUInt8Color MELUInt8ColorBlendWithColor(MELUInt8Color self, MELUInt8Color incoming) {
    const uint oneMinusAlpha = 255 - incoming.alpha;
    return (MELUInt8Color) {
        MELUInt8Min(incoming.red + self.red * oneMinusAlpha / 255, 255),
        MELUInt8Min(incoming.green + self.green * oneMinusAlpha / 255, 255),
        MELUInt8Min(incoming.blue + self.blue * oneMinusAlpha / 255, 255),
        255
    };
}
