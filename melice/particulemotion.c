//
//  particulemotion.c
//  melice
//
//  Created by Raphaël Calabro on 22/04/2023.
//

#include "particulemotion.h"

#include "animation.h"

MELMotion * _Nonnull MELParticuleMotionAlloc(void) {
    MELMotion *self = malloc(sizeof(MELMotion));
    *self = (MELMotion) {
        .class = &MELParticuleMotionClass,
    };
    return self;
}

static void update(MELMotion * _Nonnull self, MELSprite * _Nonnull sprite, MELTimeInterval timeSinceLastUpdate) {
    if (MELAnimationIsLastFrame(sprite->animation)) {
        MELSpriteRemove(sprite);
    }
}

const MELMotionClass MELParticuleMotionClass = {
    .load = &MELMotionLoadUnload,
    .unload = &MELMotionLoadUnload,
    .update = &update,
};
