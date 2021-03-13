//
//  hitbox.h
//  shmup
//
//  Created by Raphaël Calabro on 27/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef hitbox_h
#define hitbox_h

#include "../../melstd.h"

#include "../../geometry/point.h"
#include "../../geometry/rectangle.h"

typedef struct melhitbox MELHitbox;

struct melhitbox {
    MELRectangle (* _Nonnull getFrame)(MELHitbox * _Nonnull self);
    void (* _Nullable deinit)(MELHitbox * _Nonnull self);
};

MELBoolean MELHitboxCollidesWithPoint(MELHitbox * _Nonnull self, MELPoint point);

MELBoolean MELHitboxCollidesWithRectangle(MELHitbox * _Nonnull self, MELRectangle rectangle);

MELBoolean MELHitboxCollidesWithHitbox(MELHitbox * _Nonnull self, MELHitbox * _Nonnull other);

void MELHitboxDeinit(MELHitbox * _Nullable hitbox);

#endif /* hitbox_h */
