//
//  mutablemap.c
//  melice
//
//  Created by Raphaël Calabro on 02/11/2021.
//

#include "mutablemap.h"

MELListImplement(MELMutableMap);

void MELMutableMapDeinit(MELMutableMap * _Nonnull self) {
    self->size = MELIntSizeZero;
    self->backgroundColor = (MELColor) {0,0,0,0};
    MELLayerListDeinitWithDeinitFunction(&self->layers, &MELLayerDeinit);
    free(self->name);
    self->name = NULL;
}

MELMap MELMutableMapToMELMap(MELMutableMap self) {
    return (MELMap) {
        self.size,
        self.backgroundColor,
        self.layers.memory,
        self.layers.count
    };
}
