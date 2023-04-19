//
//  shotmotion.c
//  shmup
//
//  Created by Raphaël Calabro on 16/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "shotmotion.h"

#include <assert.h>
#include <math.h>
#include "camera.h"

MELMotion * _Nonnull MELBulletMotionAlloc(GLfloat angle, MELPoint speed, int damage) {
    MELPoint *camera = MELCameraGetCurrent();
    assert(camera != NULL);
    
    MELBulletMotion *self = malloc(sizeof(MELBulletMotion));
    *self = (MELBulletMotion) {
        {
            &MELBulletMotionClass
        },
        angle + M_PI / 2,
        speed,
        damage,
        camera
    };
    return (MELMotion *)self;
}

void MELBulletMotionUpdate(MELBulletMotion * _Nonnull self, MELSprite * _Nonnull sprite, MELTimeInterval timeSinceLastUpdate) {
    MELRectangle frame = sprite->frame;
    
    frame.origin = MELPointAdd(frame.origin, MELPointMultiplyByValue(self->speed, (GLfloat)timeSinceLastUpdate));
    sprite->frame = frame;
    
    MELSurfaceSetVerticesWithQuadrilateral(sprite->surface, MELRectangleRotateWithPivot(frame, self->angle, frame.origin));
    MELCameraRemoveSpriteIfOutOfView(sprite);
}

const MELMotionClass MELBulletMotionClass = {
    .load = &MELMotionLoadUnload,
    .unload = &MELMotionLoadUnload,
    .update = (void (*)(MELMotion *, MELSprite *, MELTimeInterval))&MELBulletMotionUpdate,
};
