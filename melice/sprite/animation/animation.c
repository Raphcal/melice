//
//  animation.c
//  shmup
//
//  Created by Raphaël Calabro on 28/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "animation.h"

#include "noanimation.h"
#include "singleframeanimation.h"
#include "playonceanimation.h"
#include "loopinganimation.h"
#include "synchronizedanimation.h"
#include "../spritemanager.h"

MELAnimation MELAnimationMake(const MELAnimationClass * _Nonnull class, MELAnimationDefinition * _Nullable definition) {
    return (MELAnimation) {
        class,
        definition,
        0,
        {
            0,
            MELIntRectangleZero
        },
        1
    };
}

void MELAnimationUpdate(MELAnimation * _Nonnull self, MELTimeInterval timeSinceLastUpdate) {
    // Noop
}

void MELAnimationStart(MELAnimation * _Nonnull self) {
    // Noop
}

void MELAnimationDraw(MELAnimation * _Nonnull self, MELSprite * _Nonnull sprite) {
    MELSurfaceSetTexture(sprite->surface, sprite->animation->frame.atlasIndex, sprite->parent->atlas);
}

MELAnimation MELAnimationTransitionToAnimation(MELAnimation * _Nonnull self, MELAnimation nextAnimation) {
    return nextAnimation;
}

MELTimeInterval MELAnimationFramesPerSecond(MELAnimation * _Nonnull self) {
    return self->speed * self->definition->frequency;
}

void MELAnimationSetFrameIndex(MELAnimation * _Nonnull self, int index) {
    self->frameIndex = index;
    self->frame = self->definition->frames[index];
}

MELAnimation * _Nonnull MELAnimationAlloc(MELAnimationDefinition * _Nullable definition) {
    switch (definition != NULL ? definition->type : MELAnimationTypeNone) {
        case MELAnimationTypeSingleFrame:
            return MELSingleFrameAnimationAlloc(definition);
        case MELAnimationTypePlayOnce:
            return MELPlayOnceAnimationAlloc(definition, NULL);
        case MELAnimationTypeLooping:
            return MELLoopingAnimationAlloc(definition);
        case MELAnimationTypeSynchronized:
            return MELSynchronizedLoopingAnimationAlloc(definition);
        default:
            return MELNoAnimationAlloc(definition);
    }
}
