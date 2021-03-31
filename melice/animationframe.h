//
//  animationframe.h
//  shmup
//
//  Created by Raphaël Calabro on 28/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef animationframe_h
#define animationframe_h

#include "melstd.h"

#include "rectangle.h"
#include "size.h"
#include "inputstream.h"

/**
 * Frame from an animation of a sprite.
 */
typedef struct {

    /**
     * Index of the frame inside the texture atlas.
     */
    int atlasIndex;
    
    /**
     * Hitbox rectangle for this frame.
     */
    MELIntRectangle hitbox;
    
} MELAnimationFrame;

/**
 * Returns a frame with the given values.
 *
 * @param atlasIndex Index of the frame inside the texture atlas.
 * @param hitbox Hitbox rectangle for this frame.
 * @param size Size of the frame. May be different from the atlas image size if the texture is at a higher resolution.
 * @return A frame.
 */
#define MELAnimationFrameMake(atlasIndex, hitbox) ((MELAnimationFrame) {atlasIndex, hitbox})

/**
 * Reads and returns a frame from the given input stream.
 *
 * @param inputStream Input stream to read from.
 * @return A frame.
 */
MELAnimationFrame MELAnimationFrameMakeWithInputStream(MELInputStream * _Nonnull inputStream);

#endif /* animationframe_h */
