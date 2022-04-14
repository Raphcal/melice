//
//  direction.c
//  shmup
//
//  Created by Raphaël Calabro on 27/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "direction.h"

const GLfloat MELDirectionValues[] = { -1, 1, -1, 1 };

const MELDirection MELDirectionReverses[] = { MELDirectionRight, MELDirectionLeft, MELDirectionDown, MELDirectionUp };

const MELAxe MELDirectionAxes[] = { MELAxeHorizontal, MELAxeHorizontal, MELAxeVertical, MELAxeVertical };

GLfloat MELDirectionValueOf(MELDirection self) {
    return MELDirectionValues[self];
}

MELDirection MELDirectionReverseOf(MELDirection self) {
    return MELDirectionReverses[self];
}

MELAxe MELDirectionAxeOf(MELDirection self) {
    return MELDirectionAxes[self];
}
