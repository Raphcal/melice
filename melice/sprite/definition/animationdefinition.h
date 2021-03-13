//
//  animationdefinition.h
//  shmup
//
//  Created by Raphaël Calabro on 28/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef animationdefinition_h
#define animationdefinition_h

#include "../../melstd.h"

#include "../animation/animationframe.h"
#include "../animation/animationtype.h"
#include "../../io/inputstream.h"

/**
 * Definition of an animation.
 */
typedef struct {
    
    /**
     * Name of the definition.
     */
    wchar_t * _Nullable name;
    
    /**
     * Number of frames in this animation.
     */
	unsigned int frameCount;
    
    /**
     * Frames.
     */
    MELAnimationFrame * _Nullable frames;
    
    /**
     * Number of frames by seconds.
     */
    int frequency;
    
    /**
     * Type of animation.
     */
    MELAnimationType type;

} MELAnimationDefinition;

/**
 * Returns an animation definition with the content of the given input stream.
 */
MELAnimationDefinition MELAnimationDefinitionMakeWithInputStream(MELInputStream * _Nonnull inputStream);

/**
 * Returns the duration of the given animation definition.
 *
 * @param self Animation definition instance.
 * @return The duration in seconds of the given animation definition.
 */
MELTimeInterval MELAnimationDefinitionDuration(MELAnimationDefinition self);

/**
 * Deinitialize and free the resources used by the given animation definition.
 * The definition itself is not freed.
 *
 * @param self Animation definition instance.
 */
void MELAnimationDefinitionDeinit(MELAnimationDefinition * _Nonnull self);

#endif /* animationdefinition_h */
