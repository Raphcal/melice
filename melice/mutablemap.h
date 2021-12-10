//
//  mutablemap.h
//  melice
//
//  Created by Raphaël Calabro on 29/10/2021.
//

#ifndef mutablemap_h
#define mutablemap_h

#include "melstd.h"

#include "size.h"
#include "color.h"
#include "layer.h"
#include "list.h"
#include "palette.h"
#include "map.h"

typedef struct {
    MELIntSize size;
    MELColor backgroundColor;
    MELList(MELLayer) layers;
    MELPalette * _Nonnull palette;
    char * _Nullable name;
} MELMutableMap;

MELListDefine(MELMutableMap);

MELMutableMap MELMutableMapMakeWithSizeAndPalette(MELIntSize size, MELPalette * _Nonnull palette);

/**
 * NOTE: Palette instance is not duplicated and shared with the given instance.
 */
MELMutableMap MELMutableMapMakeWithMutableMap(MELMutableMap other);

/**
 * Deinitialize the given mutable map.
 *
 * The layers and the name are freed. Name is then set to NULL.
 * The size is set to zero. Background color is set to black with opacity set to zero.
 *
 * The palette is not freed because it is assumed that it may be a reference to a global object.
 *
 * @param self Instance to deinitialize.
 */
void MELMutableMapDeinit(MELMutableMap * _Nonnull self);

MELMap MELMutableMapToMELMap(MELMutableMap self);

#endif /* mutablemap_h */
