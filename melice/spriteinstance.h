//
//  spriteinstance.h
//  shmup
//
//  Created by Raphaël Calabro on 05/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef spriteinstance_h
#define spriteinstance_h

#include "melstd.h"

#include "point.h"
#include "operation.h"
#include "inputstream.h"
#include "list.h"

typedef struct {
    int32_t definitionIndex;
    MELPoint topLeft;
    MELBoolean isUnique;
    MELOperation initializationOperation;
    char * _Nullable initializationScript;
} MELSpriteInstance;

MELListDefine(MELSpriteInstance);

MELSpriteInstance MELSpriteInstanceMake(int32_t definitionIndex, MELPoint topLeft, MELBoolean isUnique, char * _Nullable initializationScript);
MELSpriteInstance MELSpriteInstanceMakeWithInputStream(MELInputStream * _Nonnull inputStream);

void MELSpriteInstanceDeinit(MELSpriteInstance * _Nonnull self);

#endif /* spriteinstance_h */
