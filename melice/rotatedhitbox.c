//
//  rotatedhitbox.c
//  shmup
//
//  Created by Raphaël Calabro on 07/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "rotatedhitbox.h"

MELRectangle MELRotatedHitboxGetFrame(MELRotatedHitbox * _Nonnull self) {
    return self->frame;
}

void MELRotatedHitboxDeinit(MELRotatedHitbox * _Nonnull self) {
    MELHitboxDeinit(self->hitbox);
    free(self->hitbox);
    self->hitbox = NULL;
}

MELHitbox * _Nonnull MELRotatedHitboxAlloc(MELHitbox * _Nonnull hitbox) {
    MELRotatedHitbox *self = malloc(sizeof(MELRotatedHitbox));
    *self = (MELRotatedHitbox) {
        {
            (MELRectangle (*)(MELHitbox *))&MELRotatedHitboxGetFrame,
            (void (*)(MELHitbox *))&MELRotatedHitboxDeinit
        },
        hitbox,
        hitbox->getFrame(hitbox)
    };
    return (MELHitbox *)self;
}

void MELRotatedHitboxRotateWithPivot(MELRotatedHitbox * _Nonnull self, GLfloat rotation, MELPoint pivot) {
    // TODO: Implémenter la rotation.
}

void MELRotatedHitboxRotateCancelRotation(MELRotatedHitbox * _Nonnull self) {
    MELHitbox *hitbox = self->hitbox;
    self->frame = hitbox->getFrame(hitbox);
}
