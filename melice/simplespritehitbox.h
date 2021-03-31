//
//  simplespritehitbox.h
//  shmup
//
//  Created by Raphaël Calabro on 07/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef simplespritehitbox_h
#define simplespritehitbox_h

#include "melstd.h"

#include "hitbox.h"

typedef struct melsprite MELSprite;

typedef struct {
    MELHitbox super;
    MELSprite * _Nonnull sprite;
} MELSimpleSpriteHitbox;

MELHitbox * _Nonnull MELSimpleSpriteHitboxAlloc(MELSprite * _Nonnull sprite);

#endif /* simplespritehitbox_h */
