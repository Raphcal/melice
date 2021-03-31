//
//  statichitbox.h
//  shmup
//
//  Created by Raphaël Calabro on 07/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef statichitbox_h
#define statichitbox_h

#include "melstd.h"

#include "hitbox.h"

typedef struct {
    MELHitbox super;
    MELRectangle frame;
} MELStaticHitbox;

MELHitbox * _Nonnull MELStaticHitboxAllocate(MELRectangle frame);

#endif /* statichitbox_h */
