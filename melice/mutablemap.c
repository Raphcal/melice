//
//  mutablemap.c
//  melice
//
//  Created by Raphaël Calabro on 02/11/2021.
//

#include "mutablemap.h"

#include "melstring.h"

MELListImplement(MELMutableMap);

const MELMutableMap MELMutableMapEmpty = {};

MELMutableMap MELMutableMapMakeWithSizeAndPalette(MELIntSize size, MELPalette * _Nonnull palette) {
    MELMutableMap self = {{size, {1,1,1,1}, MELLayerListMakeWithInitialCapacity(1)}, palette, NULL};
    MELLayerListPush(&self.super.layers, MELLayerMakeWithSize(size));
    return self;
}

MELMutableMap MELMutableMapMakeWithMutableMap(MELMutableMap other) {
    MELMutableMap self = other;
    self.name = MELStringCopy(other.name);
    self.super.layers = MELLayerListMakeWithListAndCopyFunction(other.super.layers, &MELLayerMakeWithLayer);
    return self;
}

void MELMutableMapDeinit(MELMutableMap * _Nonnull self) {
    MELMapDeinit(&self->super);
    free(self->name);
    self->name = NULL;
}
