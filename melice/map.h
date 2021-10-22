//
//  map.h
//  shmup
//
//  Created by Raphaël Calabro on 30/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef map_h
#define map_h

#include "melstd.h"

#include "layer.h"
#include "color.h"
#include "inputstream.h"

typedef struct {
    MELIntSize size;
    MELColor backgroundColor;
    int layerCount;
    MELLayer * _Nullable layers;
} MELMap;

/**
 * Returns a map with the content of the given input stream.
 *
 * @return A map.
 */
MELMap MELMapMakeWithInputStream(MELInputStream * _Nonnull inputStream);

/**
 * Deinitialize and free the resources used by the given map.
 * The map itself is not freed.
 *
 * @param self Map instance.
 */
void MELMapDeinit(MELMap * _Nonnull self);

#endif /* map_h */
