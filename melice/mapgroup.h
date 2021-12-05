//
//  mapgroup.h
//  melice
//
//  Created by Raphaël Calabro on 05/12/2021.
//

#ifndef mapgroup_h
#define mapgroup_h

#include "melstd.h"

#include "list.h"
#include "mutablemap.h"
#include "spritedefinition.h"

typedef struct melmapgroup MELMapGroup;

MELListDefine(MELMapGroup);

typedef struct melmapgroup {
    char * _Nullable name;
    MELList(MELMutableMap) maps;
    MELList(MELSpriteDefinition) sprites;
    MELMapGroupList children;
} MELMapGroup;

extern const MELMapGroup MELMapGroupEmpty;

void MELMapGroupDeinit(MELMapGroup * _Nonnull self);

#endif /* mapgroup_h */
