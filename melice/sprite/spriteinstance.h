//
//  spriteinstance.h
//  shmup
//
//  Created by Raphaël Calabro on 05/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef spriteinstance_h
#define spriteinstance_h

#include "../melstd.h"

#include "../geometry/point.h"
#include "../util/operation.h"
#include "../io/inputstream.h"

typedef struct {
    int32_t definitionIndex;
    MELPoint topLeft;
    MELBoolean isUnique;
    MELOperation initializationScript;
} MELSpriteInstance;

MELSpriteInstance MELSpriteInstanceMakeWithInputStream(MELInputStream * _Nonnull inputStream);

void MELSpriteInstanceDeinit(MELSpriteInstance * _Nonnull self);

#endif /* spriteinstance_h */
