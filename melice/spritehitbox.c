
//
//  spritehitbox.c
//  shmup
//
//  Created by Raphaël Calabro on 07/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "spritehitbox.h"

#include "sprite.h"
#include "animation.h"

MELRectangle MELSpriteHitboxGetFrame(MELSpriteHitbox * _Nonnull self) {
    MELSprite * _Nonnull sprite = self->sprite;
    MELRectangle frame = sprite->frame;
    MELIntRectangle animationFrameHitbox = sprite->animation->frame.hitbox;
    const GLfloat offsetX = (animationFrameHitbox.origin.x - frame.size.width / 2) * MELDirectionValues[sprite->direction];
    const GLfloat offsetY = animationFrameHitbox.origin.y - frame.size.height / 2;
    return MELRectangleMake(frame.origin.x + offsetX, frame.origin.y + offsetY, animationFrameHitbox.size.width, animationFrameHitbox.size.height);
}

MELHitbox * _Nonnull MELSpriteHitboxAlloc(MELSprite * _Nonnull sprite) {
    MELSpriteHitbox *self = malloc(sizeof(MELSpriteHitbox));
    *self = (MELSpriteHitbox) {
        {
            (MELRectangle (*)(MELHitbox *))&MELSpriteHitboxGetFrame,
            NULL
        },
        sprite
    };
    return (MELHitbox *)self;
}
