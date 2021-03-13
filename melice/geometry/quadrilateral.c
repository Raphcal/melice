//
//  quadrilateral.c
//  shmup
//
//  Created by Raphaël Calabro on 17/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "quadrilateral.h"

#include "rectangle.h"
#include "../util/melmath.h"

MELQuadrilateral MELQuadrilateralMake(MELPoint topLeft, MELPoint topRight, MELPoint bottomLeft, MELPoint bottomRight) {
    return (MELQuadrilateral) {
        topLeft,
        topRight,
        bottomLeft,
        bottomRight
    };
}

GLfloat MELQuadrilateralGetLeft(MELQuadrilateral self) {
    return MELFloatMin(self.topLeft.x, MELFloatMin(self.topRight.x, MELFloatMin(self.bottomLeft.x, self.bottomRight.x)));
}

GLfloat MELQuadrilateralGetRight(MELQuadrilateral self) {
    return MELFloatMax(self.topLeft.x, MELFloatMax(self.topRight.x, MELFloatMax(self.bottomLeft.x, self.bottomRight.x)));
}

GLfloat MELQuadrilateralGetTop(MELQuadrilateral self) {
    return MELFloatMin(self.topLeft.y, MELFloatMin(self.topRight.y, MELFloatMin(self.bottomLeft.y, self.bottomRight.y)));
}

GLfloat MELQuadrilateralGetBottom(MELQuadrilateral self) {
    return MELFloatMax(self.topLeft.y, MELFloatMax(self.topRight.y, MELFloatMax(self.bottomLeft.y, self.bottomRight.y)));
}

MELRectangle MELQuadrilateralGetEnclosingRectangle(MELQuadrilateral self) {
    const GLfloat left = MELQuadrilateralGetLeft(self);
    const GLfloat right = MELQuadrilateralGetRight(self);
    const GLfloat top = MELQuadrilateralGetTop(self);
    const GLfloat bottom = MELQuadrilateralGetBottom(self);
    return MELRectangleMake(left, top, right - left, bottom - top);
}
