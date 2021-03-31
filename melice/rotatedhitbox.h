//
//  rotatedhitbox.h
//  shmup
//
//  Created by Raphaël Calabro on 07/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef rotatedhitbox_h
#define rotatedhitbox_h

#include "melstd.h"

#include "hitbox.h"
#include "point.h"

typedef struct {
    MELHitbox super;
    MELHitbox * _Nullable hitbox;
    MELRectangle frame;
} MELRotatedHitbox;

MELHitbox * _Nonnull MELRotatedHitboxAlloc(MELHitbox * _Nonnull hitbox);

void MELRotatedHitboxRotateWithPivot(MELRotatedHitbox * _Nonnull self, GLfloat rotation, MELPoint pivot);

void MELRotatedHitboxRotateCancelRotation(MELRotatedHitbox * _Nonnull self);

#endif /* rotatedhitbox_h */
