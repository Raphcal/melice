//
//  shotmotion.h
//  shmup
//
//  Created by Raphaël Calabro on 16/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef bulletmotion_h
#define bulletmotion_h

#include "melstd.h"

#include "motion.h"
#include "point.h"

extern const MELMotionClass MELBulletMotionClass;

typedef struct {
    MELMotion super;
    GLfloat angle;
    MELPoint speed;
    int damage;
} MELBulletMotion;

MELMotion * _Nonnull MELBulletMotionAlloc(GLfloat angle, MELPoint speed, int damage);

#endif /* bulletmotion_h */
