//
//  spritehitbox.h
//  shmup
//
//  Created by Raphaël Calabro on 07/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef spritehitbox_h
#define spritehitbox_h

#include "../../melstd.h"

#include "hitbox.h"

typedef struct melsprite MELSprite;

typedef struct {
    MELHitbox super;
    MELSprite * _Nonnull sprite;
} MELSpriteHitbox;

MELHitbox * _Nonnull MELSpriteHitboxAlloc(MELSprite * _Nonnull sprite);

#endif /* spritehitbox_h */
