//
//  mapgroup.c
//  melice
//
//  Created by Raphaël Calabro on 05/12/2021.
//

#include "mapgroup.h"

#include "project.h"

MELListImplement(MELMapGroup);

const MELMapGroup MELMapGroupEmpty = {NULL, {NULL, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}};

MELMapGroup MELMapGroupMakeWithMapGroup(MELMapGroup other) {
    MELMapGroup self;
    self.name = MELStringCopy(other.name);
    self.maps = MELMutableMapListMakeWithListAndCopyFunction(other.maps, &MELMutableMapMakeWithMutableMap);
    self.sprites = MELSpriteDefinitionListMakeWithListAndCopyFunction(other.sprites, &MELSpriteDefinitionMakeWithSpriteDefinition);
    self.children = MELMapGroupListMakeWithListAndCopyFunction(other.children, &MELMapGroupMakeWithMapGroup);
    return self;
}

MELMapGroup MELMapGroupMakeWithMapGroupAndProject(MELMapGroup other, MELProject sourceProject, MELProject targetProject) {
    MELMapGroup self;
    self.name = MELStringCopy(other.name);
    self.maps = MELMutableMapListMakeWithInitialCapacity(other.maps.count);
    for (unsigned int index = 0; index < other.maps.count; index++) {
        MELMutableMap map = MELMutableMapMakeWithMutableMap(other.maps.memory[index]);
        int paletteIndex = MELPaletteRefListIndexOf(sourceProject.palettes, map.palette);
        if (paletteIndex >= 0) {
            map.palette = targetProject.palettes.memory[paletteIndex];
        } else {
            fprintf(stderr, "Unable to find palette %s of map %d %s in source project.\n", map.palette != NULL ? map.palette->name : "(palette is null)", index, map.name);
        }
        self.maps.memory[index] = map;
    }
    MELMutableMapListMakeWithListAndCopyFunction(other.maps, &MELMutableMapMakeWithMutableMap);
    self.sprites = MELSpriteDefinitionListMakeWithListAndCopyFunction(other.sprites, &MELSpriteDefinitionMakeWithSpriteDefinition);
    self.children = MELMapGroupListMakeWithInitialCapacity(other.children.count);
    for (unsigned int index = 0; index < other.children.count; index++) {
        self.children.memory[index] = MELMapGroupMakeWithMapGroupAndProject(other.children.memory[index], sourceProject, targetProject);
    }
    return self;
}

void MELMapGroupDeinit(MELMapGroup * _Nonnull self) {
    free(self->name);
    self->name = NULL;
    MELMutableMapListDeinitWithDeinitFunction(&self->maps, &MELMutableMapDeinit);
    MELSpriteDefinitionListDeinitWithDeinitFunction(&self->sprites, &MELSpriteDefinitionDeinit);
    MELMapGroupListDeinitWithDeinitFunction(&self->children, &MELMapGroupDeinit);
}
