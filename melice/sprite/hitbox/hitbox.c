//
//  hitbox.c
//  shmup
//
//  Created by Raphaël Calabro on 27/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "hitbox.h"

#define _USE_MATH_DEFINES
#include <math.h>

MELBoolean MELHitboxCollidesWithPoint(MELHitbox * _Nonnull self, MELPoint point) {
    const MELRectangle frame = self->getFrame(self);
    return point.x >= MELRectangleOriginIsCenterGetLeft(frame) &&
        point.x < MELRectangleOriginIsCenterGetRight(frame) &&
        point.y >= MELRectangleOriginIsCenterGetTop(frame) &&
        point.y < MELRectangleOriginIsCenterGetBottom(frame);
}

MELBoolean MELHitboxCollidesWithRectangle(MELHitbox * _Nonnull self, MELRectangle rectangle) {
    const MELRectangle frame = self->getFrame(self);
    const MELBoolean x = fabsf(frame.origin.x - rectangle.origin.x) <= (frame.size.width + rectangle.size.width) / 2;
    const MELBoolean y = fabsf(frame.origin.y - rectangle.origin.y) <= (frame.size.height + rectangle.size.height) / 2;
    return x && y;
}

MELBoolean MELHitboxCollidesWithHitbox(MELHitbox * _Nonnull self, MELHitbox * _Nonnull other) {
    const MELRectangle frame = self->getFrame(self);
    const MELRectangle otherFrame = other->getFrame(other);
    const MELBoolean x = fabsf(frame.origin.x - otherFrame.origin.x) <= (frame.size.width + otherFrame.size.width) / 2;
    const MELBoolean y = fabsf(frame.origin.y - otherFrame.origin.y) <= (frame.size.height + otherFrame.size.height) / 2;
    return x && y;
}

void MELHitboxDeinit(MELHitbox * _Nullable hitbox) {
    if (hitbox != NULL && hitbox->deinit != NULL) {
        hitbox->deinit(hitbox);
    }
}
