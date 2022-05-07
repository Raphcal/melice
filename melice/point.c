//
//  point.c
//  shmup
//
//  Created by Raphaël Calabro on 24/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "point.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include "size.h"

const MELPoint MELPointZero = {0, 0};
const MELIntPoint MELIntPointZero = {0, 0};

MELPoint MELPointMake(GLfloat x, GLfloat y) {
    return (MELPoint) {
        x,
        y
    };
}

MELIntPoint MELIntPointMake(int32_t x, int32_t y) {
    return (MELIntPoint) {
        x,
        y
    };
}

MELBoolean MELPointEquals(MELPoint lhs, MELPoint rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

MELBoolean MELIntPointEquals(MELIntPoint lhs, MELIntPoint rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

MELPoint MELPointAdd(MELPoint lhs, MELPoint rhs) {
    return (MELPoint) {
        lhs.x + rhs.x,
        lhs.y + rhs.y
    };
}

MELIntPoint MELIntPointAdd(MELIntPoint lhs, MELIntPoint rhs) {
    return (MELIntPoint) {
        lhs.x + rhs.x,
        lhs.y + rhs.y
    };
}

MELPoint MELPointAddSize(MELPoint point, MELSize size) {
    return (MELPoint) {
        point.x + size.width,
        point.y + size.height
    };
}

MELPoint MELPointAddValue(MELPoint point, GLfloat value) {
    return (MELPoint) {
        point.x + value,
        point.y + value
    };
}

MELPoint MELPointSubstract(MELPoint lhs, MELPoint rhs) {
    return (MELPoint) {
        lhs.x - rhs.x,
        lhs.y - rhs.y
    };
}

MELIntPoint MELIntPointSubstract(MELIntPoint lhs, MELIntPoint rhs) {
    return (MELIntPoint) {
        lhs.x - rhs.x,
        lhs.y - rhs.y
    };
}

MELPoint MELPointSubstractSize(MELPoint point, MELSize size) {
    return (MELPoint) {
        point.x - size.width,
        point.y - size.height
    };
}

MELPoint MELPointSubstractValue(MELPoint point, GLfloat value) {
    return (MELPoint) {
        point.x - value,
        point.y - value
    };
}

MELPoint MELPointMultiply(MELPoint lhs, MELPoint rhs) {
    return (MELPoint) {
        lhs.x * rhs.x,
        lhs.y * rhs.y
    };
}

MELPoint MELPointMultiplyByValue(MELPoint self, GLfloat value) {
    return (MELPoint) {
        self.x * value,
        self.y * value
    };
}

MELPoint MELPointDivide(MELPoint lhs, MELPoint rhs) {
    return (MELPoint) {
        rhs.x != 0 ? lhs.x / rhs.x : 0,
        rhs.y != 0 ? lhs.y / rhs.y : 0
    };
}

MELPoint MELPointDivideByValue(MELPoint self, GLfloat value) {
    if (value == 0) {
        return MELPointZero;
    } else {
        return (MELPoint) {
            self.x / value,
            self.y / value
        };
    }
}

GLfloat MELPointDistanceToPoint(MELPoint self, MELPoint other) {
    return hypotf(self.x - other.x, self.y - other.y);
}

GLfloat MELPointAngleToPoint(MELPoint self, MELPoint other) {
    return atan2f(self.y - other.y, self.x - other.x);
}
