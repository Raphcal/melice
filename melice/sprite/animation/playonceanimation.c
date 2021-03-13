//
//  playonceanimation.c
//  shmup
//
//  Created by Raphaël Calabro on 05/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "playonceanimation.h"

void MELPlayOnceAnimationStart(MELPlayOnceAnimation * _Nonnull self) {
    MELAnimationSetFrameIndex((MELAnimation *)self, 0);
    self->startDate = time(NULL);
}

void MELPlayOnceAnimationUpdate(MELPlayOnceAnimation * _Nonnull self, MELTimeInterval timeSinceLastUpdate) {
    const time_t timeSinceStart = time(NULL) - self->startDate;
    const MELTimeInterval framesPerSecond = MELAnimationFramesPerSecond((MELAnimation *)self);
    const int frame = (int)(timeSinceStart / 1000.0 * framesPerSecond);
    
    const int frameCount = self->super.definition->frameCount;
    if (frame < frameCount) {
        MELAnimationSetFrameIndex((MELAnimation *)self, frame);
    } else {
        MELAnimationSetFrameIndex((MELAnimation *)self, frameCount - 1);
        
        MELPlayOnceAnimationOnEnd onEnd = self->onEnd;
        if (onEnd != NULL) {
            self->onEnd = NULL;
            onEnd();
        }
    }
}

const MELAnimationClass MELPlayOnceAnimationClass = {
    MELAnimationClassDefaults,
    .start = (void (*)(MELAnimation *)) &MELPlayOnceAnimationStart,
    .update = (void (*)(MELAnimation *, MELTimeInterval)) &MELPlayOnceAnimationUpdate
};

MELAnimation * _Nonnull MELPlayOnceAnimationAlloc(MELAnimationDefinition * _Nonnull definition, MELPlayOnceAnimationOnEnd onEnd) {
    MELPlayOnceAnimation *self = malloc(sizeof(MELPlayOnceAnimation));
    *self = (MELPlayOnceAnimation) {
        MELAnimationMake(&MELPlayOnceAnimationClass, definition),
        onEnd,
        0
    };
    return (MELAnimation *)self;
}
