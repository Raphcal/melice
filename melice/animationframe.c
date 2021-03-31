//
//  animationframe.c
//  shmup
//
//  Created by Raphaël Calabro on 28/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "animationframe.h"

#include <assert.h>

MELAnimationFrame MELAnimationFrameMakeWithInputStream(MELInputStream * _Nonnull inputStream) {
    assert(inputStream->file != NULL);

    MELAnimationFrame self;
    self.atlasIndex = MELInputStreamReadInt(inputStream);
    const MELBoolean hasHitbox = MELInputStreamReadBoolean(inputStream);
    MELIntRectangle hitbox;
    if (hasHitbox) {
        hitbox.origin.x = MELInputStreamReadInt(inputStream);
        hitbox.origin.y = MELInputStreamReadInt(inputStream);
        hitbox.size.width = MELInputStreamReadInt(inputStream);
        hitbox.size.height = MELInputStreamReadInt(inputStream);
    } else {
        hitbox = MELIntRectangleZero;
    }
    self.hitbox = hitbox;
    return self;
}
