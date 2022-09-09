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

typedef struct melproject MELProject;
typedef struct melmapgroup MELMapGroup;

MELListDefine(MELMapGroup);

typedef struct melmapgroup {
    char * _Nullable name;
    MELList(MELMutableMap) maps;
    MELList(MELSpriteDefinition) sprites;
    MELMapGroupList children;
} MELMapGroup;

extern const MELMapGroup MELMapGroupEmpty;

/**
 * Creates a new map group by copying the content of the given map group.
 * Palettes are not copied, copied maps will use the same palettes than the given maps.
 *
 * @param other Map group to copy.
 * @returns A new map group.
 * @deprecated Replaced by MELMapGroupMakeWithMapGroupAndProject.
 */
__attribute__((deprecated)) MELMapGroup MELMapGroupMakeWithMapGroup(MELMapGroup other);

/**
 * Creates a new map group by copying the content of the given map group.
 * Palettes wil be changed to use the one from `targetProject`.
 *
 * @param other Map group to copy.
 * @param sourceProject Project being copied.
 * @param targetProject Project copy.
 * @returns A new map group.
 */
MELMapGroup MELMapGroupMakeWithMapGroupAndProject(MELMapGroup other, MELProject sourceProject, MELProject targetProject);

void MELMapGroupDeinit(MELMapGroup * _Nonnull self);

#endif /* mapgroup_h */
