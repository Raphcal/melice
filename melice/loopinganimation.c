//
//  loopinganimation.c
//  shmup
//
//  Created by Raphaël Calabro on 05/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "loopinganimation.h"

void MELLoopingAnimationStart(MELLoopingAnimation * _Nonnull self) {
    MELAnimationSetFrameIndex((MELAnimation *)self, 0);
    self->time = 0;
}

void MELLoopingAnimationUpdate(MELLoopingAnimation * _Nonnull self, MELTimeInterval timeSinceLastUpdate) {
    MELTimeInterval time = self->time + timeSinceLastUpdate;
    
    const MELTimeInterval framesPerSecond = MELAnimationFramesPerSecond((MELAnimation *)self);
    const int elapsedFrames = (int)(time * framesPerSecond);
    if (elapsedFrames > 0) {
        MELAnimationSetFrameIndex((MELAnimation *)self, (self->super.frameIndex + elapsedFrames) % self->super.definition->frameCount);
        time -= elapsedFrames / framesPerSecond;
    }
    self->time = time;
}

const MELAnimationClass MELLoopingAnimationClass = {
    MELAnimationClassDefaults,
    .start = (void (*)(MELAnimation *)) &MELLoopingAnimationStart,
    .update = (void (*)(MELAnimation *, MELTimeInterval)) &MELLoopingAnimationUpdate
};

MELAnimation * _Nonnull MELLoopingAnimationAlloc(MELAnimationDefinition * _Nonnull definition) {
    MELLoopingAnimation *self = malloc(sizeof(MELLoopingAnimation));
    *self = (MELLoopingAnimation) {
        MELAnimationMake(&MELLoopingAnimationClass, definition),
        0
    };
    return (MELAnimation *)self;
}
