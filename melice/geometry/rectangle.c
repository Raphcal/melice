//
//  rectangle.c
//  shmup
//
//  Created by Raphaël Calabro on 24/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "rectangle.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "quadrilateral.h"
#include "../util/list.h"

const MELRectangle MELRectangleZero = { {0, 0}, {0, 0} };
const MELIntRectangle MELIntRectangleZero = { {0, 0}, {0, 0} };

MELRectangle MELRectangleMake(GLfloat x, GLfloat y, GLfloat width, GLfloat height) {
    return (MELRectangle) {
        {
            x,
            y
        },
        {
            width,
            height
        }
    };
}

MELRectangle MELRectangleMakeWithOriginAndSize(MELPoint origin, MELSize size) {
    return (MELRectangle) {
        origin,
        size
    };
}

MELIntRectangle MELIntRectangleMake(int32_t x, int32_t y, int32_t width, int32_t height) {
    return (MELIntRectangle) {
        {
            x,
            y
        },
        {
            width,
            height
        }
    };
}

MELIntRectangle MELIntRectangleMakeWithOriginAndSize(MELIntPoint origin, MELIntSize size) {
    return (MELIntRectangle) {
        origin,
        size
    };
}

GLfloat MELRectangleOriginIsCenterGetLeft(MELRectangle self) {
    return self.origin.x - self.size.width / 2;
}

GLfloat MELRectangleOriginIsCenterGetRight(MELRectangle self) {
    return self.origin.x + self.size.width / 2;
}

GLfloat MELRectangleOriginIsCenterGetTop(MELRectangle self) {
    return self.origin.y - self.size.height / 2;
}

GLfloat MELRectangleOriginIsCenterGetBottom(MELRectangle self) {
    return self.origin.y + self.size.height / 2;
}

MELBoolean MELIntRectangleEquals(MELIntRectangle lhs, MELIntRectangle rhs) {
    return lhs.origin.x == rhs.origin.x &&
        lhs.origin.y == rhs.origin.y &&
        lhs.size.width == rhs.size.width &&
        lhs.size.height == rhs.size.height;
}

MELQuadrilateral MELRectangleRotate(MELRectangle self, GLfloat rotation) {
    return MELRectangleRotateWithPivot(self, rotation, self.origin);
}

MELQuadrilateral MELRectangleRotateWithPivot(MELRectangle self, GLfloat rotation, MELPoint pivot) {
    MELPoint vertices[4];
    const GLfloat left = MELRectangleOriginIsCenterGetLeft(self);
    const GLfloat right = MELRectangleOriginIsCenterGetRight(self);
    const GLfloat top = MELRectangleOriginIsCenterGetTop(self);
    const GLfloat bottom = MELRectangleOriginIsCenterGetBottom(self);
    MELPoint source[4] = {
        MELPointMake(left, top),
        MELPointMake(right, top),
        MELPointMake(left, bottom),
        MELPointMake(right, bottom)
    };
    for (int index = 0; index < 4; index++) {
        const MELPoint vertex = source[index];
        const GLfloat length = MELPointDistanceToPoint(vertex, pivot);
        const GLfloat angle = MELPointAngleToPoint(vertex, pivot);
        vertices[index] = MELPointMake(pivot.x + cosf(angle + rotation) * length, pivot.y + sinf(angle + rotation) * length);
    }
    return MELQuadrilateralMake(vertices[0], vertices[1], vertices[2], vertices[3]);
}
