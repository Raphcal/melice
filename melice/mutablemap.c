//
//  mutablemap.c
//  melice
//
//  Created by Raphaël Calabro on 02/11/2021.
//

#include "mutablemap.h"

#include "melstring.h"

MELListImplement(MELMutableMap);
MELListImplementGetSetByUUID(MELMutableMap);

const MELMutableMap MELMutableMapEmpty = {};

MELMutableMap MELMutableMapMakeWithSizeAndPalette(MELIntSize size, MELPalette * _Nonnull palette) {
    MELMutableMap self;
    self.super.size = size;
    self.super.backgroundColor = (MELColor) {1,1,1,1};
    self.super.layers = MELLayerListMakeWithInitialCapacity(1);
    self.palette = palette;
    uuid_generate(self.uuid);
    MELLayerListPush(&self.super.layers, MELLayerMakeWithSize(size));
    return self;
}

MELMutableMap MELMutableMapMakeWithMutableMap(MELMutableMap other) {
    MELMutableMap self = other;
    self.name = MELStringCopy(other.name);
    self.super.layers = MELLayerListMakeWithListAndCopyFunction(other.super.layers, &MELLayerMakeWithLayer);
    uuid_generate(self.uuid);
    return self;
}

void MELMutableMapDeinit(MELMutableMap * _Nonnull self) {
    MELMapDeinit(&self->super);
    free(self->name);
    self->name = NULL;
    uuid_clear(self->uuid);
}
