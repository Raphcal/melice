//
//  surfacememory.h
//  shmup
//
//  Created by Raphaël Calabro on 28/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef surfacememory_h
#define surfacememory_h

#include "../melstd.h"

#include "color.h"
#include "../geometry/rectangle.h"
#include "../geometry/quadrilateral.h"

void MELSurfaceMemoryAppendRectangle(GLfloat * _Nonnull memory, MELRectangle rectangle);

void MELSurfaceMemoryAppendQuadrilateral(GLfloat * _Nonnull memory, MELQuadrilateral quadrilateral);

void MELSurfaceMemoryAppendColor(GLubyte * _Nonnull memory, MELUInt8Color color);

#endif /* surfacememory_h */
