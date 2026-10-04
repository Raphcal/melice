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
#include "shootingstyledefinition.h"

extern const MELMotionClass MELBulletMotionClass;

typedef struct {
    MELMotion super;
    MELPoint from;
    MELShootingStyleEasingFunction easingFunction;
    MELTimeInterval time;
    float angle;
    MELPoint speed;
    int damage;
} MELBulletMotion;

MELMotion * _Nonnull MELBulletMotionAlloc(MELPoint origin, GLfloat angle, MELPoint speed, int damage, MELShootingStyleEasingFunction easingFunction);

#endif /* bulletmotion_h */
