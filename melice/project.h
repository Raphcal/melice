//
//  project.h
//  melice
//
//  Created by Raphaël Calabro on 29/10/2021.
//

#ifndef project_h
#define project_h

#include "melstd.h"

#include "list.h"
#include "palette.h"
#include "mutablemap.h"
#include "spritedefinition.h"

typedef struct melmapgroup MELMapGroup;

MELListDefine(MELMapGroup);

typedef struct {
    MELList(MELPaletteRef) palettes;
    MELList(MELMapGroup) mapGroups;
} MELProject;

typedef struct melmapgroup {
    uint8_t * _Nullable name;
    MELList(MELSpriteDefinition) sprites;
    MELList(MELMutableMap) maps;
    MELMapGroup * _Nullable children;
} MELMapGroup;

#endif /* project_h */
