//
//  size.c
//  shmup
//
//  Created by Raphaël Calabro on 24/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "size.h"

const MELSize MELSizeZero = {0, 0};
const MELIntSize MELIntSizeZero = {0, 0};

MELSize MELSizeMake(GLfloat width, GLfloat height) {
    return (MELSize) {
        width,
        height
    };
}

MELIntSize MELIntSizeMake(int32_t width, int32_t height) {
    return (MELIntSize) {
        width,
        height
    };
}

MELSize MELSizeAdd(MELSize lhs, MELSize rhs) {
    return (MELSize) {lhs.width + rhs.width, lhs.height + rhs.height};
}
