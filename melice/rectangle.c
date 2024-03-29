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
#include "list.h"

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

MELRectangle MELRectangleMakeWithPoints(MELPoint a, MELPoint b) {
    MELRectangle self;
    if (a.x < b.x) {
        self.origin.x = a.x;
        self.size.width = b.x - a.x;
    } else {
        self.origin.x = b.x;
        self.size.width = a.x - b.x;
    }
    if (a.y < b.y) {
        self.origin.y = a.y;
        self.size.height = b.y - a.y;
    } else {
        self.origin.y = b.y;
        self.size.height = a.y - b.y;
    }
    return self;
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

MELIntRectangle MELIntRectangleMakeWithPoints(MELIntPoint a, MELIntPoint b) {
    MELIntRectangle self;
    if (a.x < b.x) {
        self.origin.x = a.x;
        self.size.width = b.x - a.x;
    } else {
        self.origin.x = b.x;
        self.size.width = a.x - b.x;
    }
    if (a.y < b.y) {
        self.origin.y = a.y;
        self.size.height = b.y - a.y;
    } else {
        self.origin.y = b.y;
        self.size.height = a.y - b.y;
    }
    return self;
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

MELBoolean MELRectangleEquals(MELRectangle lhs, MELRectangle rhs) {
    return lhs.origin.x == rhs.origin.x &&
        lhs.origin.y == rhs.origin.y &&
        lhs.size.width == rhs.size.width &&
        lhs.size.height == rhs.size.height;
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

MELBoolean MELRectangleContainsPoint(MELRectangle self, MELPoint point) {
    return point.x >= self.origin.x &&
        point.x < self.origin.x + self.size.width &&
        point.y >= self.origin.y &&
        point.y < self.origin.y + self.size.height;
}
MELBoolean MELIntRectangleContainsPoint(MELIntRectangle self, MELIntPoint point) {
    return point.x >= self.origin.x &&
        point.x < self.origin.x + self.size.width &&
        point.y >= self.origin.y &&
        point.y < self.origin.y + self.size.height;
}

MELBoolean MELRectangleIntersectsWithRectangle(MELRectangle self, MELRectangle other) {
    const MELBoolean x = fabsf(self.origin.x + self.size.width / 2 - (other.origin.x + other.size.width / 2)) <= (self.size.width + other.size.width) / 2;
    const MELBoolean y = fabsf(self.origin.y + self.size.height / 2 - (other.origin.y + other.size.height / 2)) <= (self.size.height + other.size.height) / 2;
    return x && y;
}
MELBoolean MELIntRectangleIntersectsWithRectangle(MELIntRectangle self, MELIntRectangle other) {
    const MELBoolean x = abs(self.origin.x + self.size.width / 2 - (other.origin.x + other.size.width / 2)) <= (self.size.width + other.size.width) / 2;
    const MELBoolean y = abs(self.origin.y + self.size.height / 2 - (other.origin.y + other.size.height / 2)) <= (self.size.height + other.size.height) / 2;
    return x && y;
}
