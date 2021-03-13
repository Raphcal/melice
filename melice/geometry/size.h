//
//  size.h
//  shmup
//
//  Created by Raphaël Calabro on 24/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef size_h
#define size_h

#include "../melstd.h"

typedef struct {
    GLfloat width;
    GLfloat height;
} MELSize;

typedef struct {
    int32_t width;
    int32_t height;
} MELIntSize;

/**
 * A size whose width and height are both zero.
 */
extern const MELSize MELSizeZero;

/**
 * A size whose width and height are both zero.
 */
extern const MELIntSize MELIntSizeZero;

/**
 * Returns a size with the given width and height.
 *
 * @param width Width.
 * @param height Height.
 * @return A size.
 */
MELSize MELSizeMake(GLfloat width, GLfloat height);

/**
 * Returns a size with the given width and height.
 *
 * @param width Width.
 * @param height Height.
 * @return A size.
 */
MELIntSize MELIntSizeMake(int32_t width, int32_t height);

#endif /* size_h */
