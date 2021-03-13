//
//  statichitbox.c
//  shmup
//
//  Created by Raphaël Calabro on 07/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "statichitbox.h"

MELRectangle MELStaticHitboxGetFrame(MELStaticHitbox * _Nonnull self) {
    return self->frame;
}

MELHitbox * _Nonnull MELStaticHitboxAllocate(MELRectangle frame) {
    MELStaticHitbox *self = malloc(sizeof(MELStaticHitbox));
    *self = (MELStaticHitbox) {
        {
            (MELRectangle (*)(MELHitbox *))&MELStaticHitboxGetFrame,
            NULL
        },
        frame
    };
    return (MELHitbox *)self;
}
