//
//  mapgroup.c
//  melice
//
//  Created by Raphaël Calabro on 05/12/2021.
//

#include "mapgroup.h"

MELListImplement(MELMapGroup);

MELMapGroup MELMapGroupMakeEmpty(void) {
    MELMapGroup self;
    uuid_generate(self.uuid);
    return self;
}

MELMapGroup MELMapGroupMakeWithMapGroup(MELMapGroup other) {
    MELMapGroup self;
    self.name = MELStringCopy(other.name);
    self.maps = MELMutableMapListMakeWithListAndCopyFunction(other.maps, &MELMutableMapMakeWithMutableMap);
    self.sprites = MELSpriteDefinitionListMakeWithListAndCopyFunction(other.sprites, &MELSpriteDefinitionMakeWithSpriteDefinition);
    self.children = MELMapGroupListMakeWithListAndCopyFunction(other.children, &MELMapGroupMakeWithMapGroup);
    uuid_generate(self.uuid);
    return self;
}

void MELMapGroupDeinit(MELMapGroup * _Nonnull self) {
    free(self->name);
    self->name = NULL;
    MELMutableMapListDeinitWithDeinitFunction(&self->maps, &MELMutableMapDeinit);
    MELSpriteDefinitionListDeinitWithDeinitFunction(&self->sprites, &MELSpriteDefinitionDeinit);
    MELMapGroupListDeinitWithDeinitFunction(&self->children, &MELMapGroupDeinit);
    uuid_clear(self->uuid);
}
