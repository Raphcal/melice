//
//  shotmotion.c
//  shmup
//
//  Created by Raphaël Calabro on 16/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "bulletmotion.h"

#include <assert.h>
#include <math.h>
#include "camera.h"
#include "shootingstyle.h"

MELMotion * _Nonnull MELBulletMotionAlloc(MELPoint origin, GLfloat angle, MELPoint speed, int damage, MELShootingStyleEasingFunction easingFunction) {
    MELBulletMotion *self = malloc(sizeof(MELBulletMotion));
    *self = (MELBulletMotion) {
        .super = {
            &MELBulletMotionClass
        },
        .from = origin,
        .angle = angle,
        .speed = speed,
        .damage = damage,
        .easingFunction = easingFunction ? easingFunction : MELShootingStyleEaseNone,
    };
    return &self->super;
}

static void update(MELMotion * _Nonnull motion, MELSprite * _Nonnull sprite, MELTimeInterval timeSinceLastUpdate) {
    MELBulletMotion *self = (MELBulletMotion *)motion;
    self->time += timeSinceLastUpdate;

    const MELPoint speed = self->speed;
    const MELPoint from = self->from;

    MELRectangle frame = sprite->frame;
    const float progress = self->easingFunction(self->time);
    frame.origin = (MELPoint) {
        .x = from.x + speed.x * progress,
        .y = from.y + speed.y * progress,
    };
    sprite->frame = frame;

    MELSurfaceSetVerticesWithQuadrilateral(sprite->surface, MELRectangleRotateWithPivot(frame, self->angle, frame.origin));
    MELCameraRemoveSpriteIfOutOfView(sprite);
}

const MELMotionClass MELBulletMotionClass = {
    .load = &MELMotionLoadUnload,
    .unload = &MELMotionLoadUnload,
    .update = &update,
};
