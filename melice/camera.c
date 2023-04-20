//
//  camera.c
//  melice
//
//  Created by Raphaël Calabro on 16/04/2023.
//

#include "camera.h"

static MELPoint * _Nullable cameraOrigin = NULL;
static MELSize cameraSize;

void MELCameraSetCurrent(MELPoint * _Nullable camera) {
    cameraOrigin = camera;
}

MELPoint * _Nullable MELCameraGetCurrent(void) {
    return cameraOrigin;
}

void MELCameraSetSize(MELSize size) {
    cameraSize = size;
}

MELBoolean MELCameraSpriteIsInView(MELSprite * _Nonnull sprite) {
    MELPoint origin = *cameraOrigin;
    return MELHitboxCollidesWithRectangle(sprite->hitbox, (MELRectangle) {
        .origin = {
            .x = origin.x + cameraSize.width / 2,
            .y = origin.y + cameraSize.height / 2,
        },
        .size = cameraSize,
    });
}

void MELCameraRemoveSpriteIfOutOfView(MELSprite * _Nonnull sprite) {
    if (!MELCameraSpriteIsInView(sprite)) {
        MELSpriteRemove(sprite);
    }
}
