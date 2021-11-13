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
    MELList(MELMutableMap) maps;
    MELList(MELSpriteDefinition) sprites;
    MELMapGroupList children;
} MELMapGroup;

extern const MELMapGroup MELMapGroupEmpty;

#endif /* project_h */
