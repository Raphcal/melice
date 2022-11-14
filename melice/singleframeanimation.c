//
//  singleframeanimation.c
//  shmup
//
//  Created by Raphaël Calabro on 30/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "singleframeanimation.h"

void MELSingleFrameAnimationStart(MELAnimation * _Nonnull self) {
    MELAnimationSetFrameIndex(self, 0);
}

const MELAnimationClass MELSingleFrameAnimationClass = {
    .start = &MELSingleFrameAnimationStart,
    .update = &MELAnimationNoopUpdate,
    .draw = &MELAnimationDraw,
    .transitionToAnimation = &MELAnimationTransitionToAnimation
};

MELAnimation MELSingleFrameAnimationMake(MELAnimationDefinition * _Nonnull definition) {
    return MELAnimationMake(&MELSingleFrameAnimationClass, definition);
}

MELAnimation * _Nonnull MELSingleFrameAnimationAlloc(MELAnimationDefinition * _Nonnull definition) {
    MELAnimation *self = malloc(sizeof(MELAnimation));
    *self = MELSingleFrameAnimationMake(definition);
    return self;
}
