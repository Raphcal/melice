//
//  simplespritehitbox.c
//  shmup
//
//  Created by Raphaël Calabro on 07/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "simplespritehitbox.h"

#include "../sprite.h"

MELRectangle MELSimpleSpriteHitboxGetFrame(MELSimpleSpriteHitbox * _Nonnull self) {
    return self->sprite->frame;
}

MELHitbox * _Nonnull MELSimpleSpriteHitboxAlloc(MELSprite * _Nonnull sprite) {
    MELSimpleSpriteHitbox *self = malloc(sizeof(MELSimpleSpriteHitbox));
    *self = (MELSimpleSpriteHitbox) {
        {
            (MELRectangle (*)(MELHitbox *))&MELSimpleSpriteHitboxGetFrame,
            NULL
        },
        sprite
    };
    return (MELHitbox *)self;
}
