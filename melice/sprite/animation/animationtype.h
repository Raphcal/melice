//
//  animationtype.h
//  shmup
//
//  Created by Raphaël Calabro on 29/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef animationtype_h
#define animationtype_h

#include "../../melstd.h"

/**
 * Type of animation.
 */
typedef enum {
    
    /**
     * Not animated.
     */
    MELAnimationTypeNone,
    
    /**
     * Animation displaying always the same frame.
     */
    MELAnimationTypeSingleFrame,
    
    /**
     * Play an animation once.
     */
    MELAnimationTypePlayOnce,
    
    /**
     * Play an animation and restart at the first frame when the animation ends.
     */
    MELAnimationTypeLooping,
    
    /**
     * Same as looping but every instance is playing the same frame at the same time.
     */
    MELAnimationTypeSynchronized

} MELAnimationType;

MELAnimationType MELAnimationTypeForFrameCountAndLooping(int frameCount, MELBoolean looping);

#endif /* animationtype_h */
