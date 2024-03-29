//
//  animationdefinition.h
//  shmup
//
//  Created by Raphaël Calabro on 28/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef animationdefinition_h
#define animationdefinition_h

#include "melstd.h"

#include "animationframe.h"
#include "animationtype.h"
#include "imagepaletteimage.h"
#include "inputstream.h"
#include "list.h"
#include "degreeanimationdefinitionframestable.h"

/**
 * Definition of an animation.
 */
typedef struct {
    /**
     * Name of the definition.
     */
    char * _Nullable name;

    MELDegreesMELAnimationDefinitionFramesTable framesByDirection;

    /**
     * Number of frames in this animation.
     *
     * @deprecated Replaced by framesByDirection.
     */
	unsigned int frameCount;
    /**
     * Frames.
     *
     * @deprecated Replaced by framesByDirection.
     */
    MELAnimationFrame * _Nullable frames;
    /**
     * Frames as editable images.
     *
     * @deprecated Replaced by framesByDirection.
     */
    MELImagePaletteImage * _Nullable images;
    /**
     * Number of frames by seconds.
     */
    int frequency;
    /**
     * Type of animation.
     */
    MELAnimationType type;
    MELBoolean isScrolling;
} MELAnimationDefinition;

MELListDefine(MELAnimationDefinition);

MELAnimationDefinition MELAnimationDefinitionMakeWithAnimationDefinition(MELAnimationDefinition other);

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

MELImagePaletteImage * _Nullable MELAnimationDefinitionFirstNonEmptyImage(MELAnimationDefinition self);

#endif /* animationdefinition_h */
