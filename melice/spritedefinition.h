//
//  spritedefinition.h
//  shmup
//
//  Created by Raphaël Calabro on 28/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef spritedefinition_h
#define spritedefinition_h

#include "melstd.h"

#include "animationdefinition.h"
#include "inputstream.h"
#include "list.h"

typedef struct {
    wchar_t * _Nullable name;
    uint32_t type;
	unsigned int animationCount;
    MELAnimationDefinition * _Nullable animations;
    wchar_t * _Nullable motionName;
} MELSpriteDefinition;

MELListDefine(MELSpriteDefinition);

MELSpriteDefinition MELSpriteDefinitionMakeWithInputStream(MELInputStream * _Nonnull inputStream);

MELList(MELSpriteDefinition) MELSpriteDefinitionListMakeWithInputStream(MELInputStream * _Nonnull inputStream);

void MELSpriteDefinitionDeinit(MELSpriteDefinition * _Nonnull self);

#endif /* spritedefinition_h */
