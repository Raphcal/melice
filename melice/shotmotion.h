//
//  shotmotion.h
//  shmup
//
//  Created by Raphaël Calabro on 16/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef shotmotion_h
#define shotmotion_h

#include "melstd.h"

#include "motion.h"
#include "point.h"

extern const MELMotionClass ShotMotionClass;

typedef struct {
    MELMotion super;
    GLfloat angle;
    MELPoint speed;
    int damage;
    MELPoint * _Nonnull camera;
} ShotMotion;

MELMotion * _Nonnull ShotMotionAlloc(GLfloat angle, MELPoint speed, int damage);

#endif /* shotmotion_h */
