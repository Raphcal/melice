//
//  map.c
//  shmup
//
//  Created by Raphaël Calabro on 30/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "map.h"

#include <assert.h>

MELMap MELMapMakeWithInputStream(MELInputStream * _Nonnull inputStream) {
    assert(inputStream->file != NULL);

    MELMap self;
    self.backgroundColor = MELInputStreamReadColor(inputStream);

    int maxWidth = 0;
    int maxHeight = 0;

    int layerCount = MELInputStreamReadInt(inputStream);
    self.layers = MELLayerListMakeWithInitialCapacity(layerCount);

    for (int index = 0; index < layerCount; index++) {
        MELLayer layer = MELLayerMakeWithInputStream(inputStream);
        maxWidth = layer.size.width > maxWidth ? layer.size.width : maxWidth;
        maxHeight = layer.size.height > maxHeight ? layer.size.height : maxHeight;
        MELLayerListPush(&self.layers, layer);
    }
    self.size = MELIntSizeMake(maxWidth, maxHeight);
    return self;
}

void MELMapDeinit(MELMap * _Nonnull self) {
    self->size = MELIntSizeZero;
    self->backgroundColor = (MELColor) {0,0,0,0};
    MELLayerListDeinitWithDeinitFunction(&self->layers, &MELLayerDeinit);
}
