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
#include "palette.h"
#include "imagepaletteimage.h"
#include "spritetype.h"

typedef struct {
    char * _Nullable name;
    MELSpriteType type;
    MELPaletteRef palette;
    MELAnimationDefinitionList animations;
    char * _Nullable motionName;
    char * _Nullable loadScript;
} MELSpriteDefinition;

MELListDefine(MELSpriteDefinition);

MELSpriteDefinition MELSpriteDefinitionMakeWithSpriteDefinition(MELSpriteDefinition other);

MELSpriteDefinition MELSpriteDefinitionMakeWithInputStream(MELInputStream * _Nonnull inputStream);

MELList(MELSpriteDefinition) MELSpriteDefinitionListMakeWithInputStream(MELInputStream * _Nonnull inputStream);

void MELSpriteDefinitionDeinit(MELSpriteDefinition * _Nonnull self);

MELImagePaletteImage * _Nullable MELSpriteDefinitionFirstNonEmptyImage(MELSpriteDefinition self);

#endif /* spritedefinition_h */
