//
//  mapgroup.c
//  melice
//
//  Created by Raphaël Calabro on 05/12/2021.
//

#include "mapgroup.h"

MELListImplement(MELMapGroup);

const MELMapGroup MELMapGroupEmpty = {NULL, {NULL, 0, 0}, {NULL, 0, 0}, {NULL, 0, 0}};

void MELMapGroupDeinit(MELMapGroup * _Nonnull self) {
    free(self->name);
    self->name = NULL;
    MELMutableMapListDeinitWithDeinitFunction(&self->maps, &MELMutableMapDeinit);
    MELSpriteDefinitionListDeinitWithDeinitFunction(&self->sprites, &MELSpriteDefinitionDeinit);
    MELMapGroupListDeinitWithDeinitFunction(&self->children, &MELMapGroupDeinit);
}
