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

MELMotion * _Nonnull MELBulletMotionAlloc(GLfloat angle, MELPoint speed, int damage) {
    MELBulletMotion *self = malloc(sizeof(MELBulletMotion));
    *self = (MELBulletMotion) {
        .super = {
            &MELBulletMotionClass
        },
        .angle = angle,
        .speed = speed,
        .damage = damage,
    };
    return &self->super;
}

static void update(MELMotion * _Nonnull motion, MELSprite * _Nonnull sprite, MELTimeInterval timeSinceLastUpdate) {
    MELBulletMotion *self = (MELBulletMotion *)motion;
    MELRectangle frame = sprite->frame;
    
    frame.origin = MELPointAdd(frame.origin, MELPointMultiplyByValue(self->speed, (GLfloat)timeSinceLastUpdate));
    sprite->frame = frame;

    MELSurfaceSetVerticesWithQuadrilateral(sprite->surface, MELRectangleRotateWithPivot(frame, self->angle, frame.origin));
    MELCameraRemoveSpriteIfOutOfView(sprite);
}

const MELMotionClass MELBulletMotionClass = {
    .load = &MELMotionLoadUnload,
    .unload = &MELMotionLoadUnload,
    .update = &update,
};
