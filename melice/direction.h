//
//  direction.h
//  shmup
//
//  Created by Raphaël Calabro on 27/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef direction_h
#define direction_h

#include "melstd.h"

#include "axe.h"

typedef enum {
    MELDirectionLeft,
    MELDirectionRight,
    MELDirectionUp,
    MELDirectionDown
} MELDirection;

extern const GLfloat MELDirectionValues[];
extern const MELDirection MELDirectionReverses[];
extern const MELAxe MELDirectionAxes[];

GLfloat MELDirectionValueOf(MELDirection self);
MELDirection MELDirectionReverseOf(MELDirection self);
MELAxe MELDirectionAxeOf(MELDirection self);

#endif /* direction_h */
