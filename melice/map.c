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
    
    self.layerCount = MELInputStreamReadInt(inputStream);
    self.layers = calloc(self.layerCount, sizeof(MELLayer));
    for (int index = 0; index < self.layerCount; index++) {
        MELLayer layer = MELLayerMakeWithInputStream(inputStream);
        maxWidth = layer.size.width > maxWidth ? layer.size.width : maxWidth;
        maxHeight = layer.size.height > maxHeight ? layer.size.height : maxHeight;
        self.layers[index] = layer;
    }
    self.size = MELIntSizeMake(maxWidth, maxHeight);
    return self;
}

void MELMapDeinit(MELMap * _Nonnull self) {
    self->size.width = 0;
    self->size.height = 0;
    self->backgroundColor = (MELColor) {0,0,0,0};
	unsigned int layerCount = self->layerCount;
    self->layerCount = 0;
    MELLayer *layers = self->layers;
    for (unsigned int index = 0; index < layerCount; index++) {
        MELLayerDeinit(layers + index);
    }
    free(self->layers);
    self->layers = NULL;
}
