//
//  noanimation.c
//  shmup
//
//  Created by Raphaël Calabro on 29/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "noanimation.h"

void MELNoAnimationDraw(MELAnimation * _Nonnull self, MELSprite * _Nonnull sprite) {
    // Noop
}

const MELAnimationClass MELNoAnimationClass = {
    .draw = &MELNoAnimationDraw,
    .update = &MELAnimationUpdate,
    .start = &MELAnimationStart,
    .transitionToAnimation = &MELAnimationTransitionToAnimation
};

MELAnimation * _Nonnull MELNoAnimationAlloc(MELAnimationDefinition * _Nullable definition) {
    MELAnimation *self = malloc(sizeof(MELAnimation));
    *self = MELAnimationMake(&MELNoAnimationClass, definition);
    return self;
}
