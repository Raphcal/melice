//
//  synchronizedanimation.c
//  shmup
//
//  Created by Raphaël Calabro on 05/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "synchronizedanimation.h"

#include "meltime.h"

/**
 * Start time shared by every instance of MELSynchronizedLoopingAnimation.
 */
int64_t MELSynchronizedLoopingAnimationReferenceDate = 0;

void MELSynchronizedLoopingAnimationStart(MELAnimation * _Nonnull self) {
    if (MELSynchronizedLoopingAnimationReferenceDate == 0) {
        MELSynchronizedLoopingAnimationReferenceDate = MELMilliTime();
    }
}

void MELSynchronizedLoopingAnimationUpdate(MELAnimation * _Nonnull self, MELTimeInterval timeSinceLastUpdate) {
    const MELTimeInterval timeSinceStart = (MELMilliTime() - MELSynchronizedLoopingAnimationReferenceDate) / 1000.0;
    const MELTimeInterval framesPerSecond = MELAnimationFramesPerSecond((MELAnimation *)self);
    MELAnimationSetFrameIndex(self, (int)(timeSinceStart * framesPerSecond) % self->definition->frameCount);
}

const MELAnimationClass MELSynchronizedLoopingAnimationClass = {
    .start = &MELSynchronizedLoopingAnimationStart,
    .update = &MELSynchronizedLoopingAnimationUpdate,
    .draw = &MELAnimationDraw,
    .transitionToAnimation = &MELAnimationTransitionToAnimation
};

MELAnimation * _Nonnull MELSynchronizedLoopingAnimationAlloc(MELAnimationDefinition * _Nonnull definition) {
    MELAnimation *self = malloc(sizeof(MELAnimation));
    *self = MELAnimationMake(&MELSynchronizedLoopingAnimationClass, definition);
    return self;
}
