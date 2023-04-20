//
//  playonceanimation.h
//  shmup
//
//  Created by Raphaël Calabro on 05/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef playonceanimation_h
#define playonceanimation_h

#include "melstd.h"

#include "animation.h"
#include "animationdefinition.h"
#include "meltime.h"

/**
 * Class of playing once animations.
 */
extern const MELAnimationClass MELPlayOnceAnimationClass;

typedef void (* _Nullable MELPlayOnceAnimationOnEnd)(void);

typedef struct {
    MELAnimation super;
    MELPlayOnceAnimationOnEnd onEnd;
    time_t startDate;
} MELPlayOnceAnimation;

MELAnimation * _Nonnull MELPlayOnceAnimationAlloc(MELAnimationDefinition * _Nonnull definition, MELPlayOnceAnimationOnEnd onEnd);

#endif /* playonceanimation_h */
