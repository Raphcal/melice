//
//  quadrilateral.h
//  shmup
//
//  Created by Raphaël Calabro on 17/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef quadrilateral_h
#define quadrilateral_h

#include "melstd.h"

#include "point.h"

typedef struct melrectangle MELRectangle;

typedef struct melquadrilateral {
    MELPoint topLeft;
    MELPoint topRight;
    MELPoint bottomLeft;
    MELPoint bottomRight;
} MELQuadrilateral;

MELQuadrilateral MELQuadrilateralMake(MELPoint topLeft, MELPoint topRight, MELPoint bottomLeft, MELPoint bottomRight);

GLfloat MELQuadrilateralGetLeft(MELQuadrilateral self);
GLfloat MELQuadrilateralGetRight(MELQuadrilateral self);
GLfloat MELQuadrilateralGetTop(MELQuadrilateral self);
GLfloat MELQuadrilateralGetBottom(MELQuadrilateral self);

MELRectangle MELQuadrilateralGetEnclosingRectangle(MELQuadrilateral self);

#endif /* quadrilateral_h */
