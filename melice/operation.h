//
//  operation.h
//  shmup
//
//  Created by Raphaël Calabro on 08/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef operation_h
#define operation_h

#include "melstd.h"

#include "list.h"
#include "inputstream.h"
#include "primitives.h"

typedef struct {
    uint8_t * _Nonnull code;
    int32_t count;
} MELOperation;

typedef struct melsprite MELSprite;

MELOperation MELOperationMakeWithInputStream(MELInputStream * _Nonnull inputStream);
void MELOperationDeinit(MELOperation * _Nonnull operation);

MELList(GLfloat) MELOperationExecute(MELOperation self, GLfloat x, MELSprite * _Nullable sprite);

#endif /* operation_h */
