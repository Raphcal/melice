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
    MELPalette * _Nullable palette;
    char * _Nullable name;
} MELMutableMap;

MELListDefine(MELMutableMap);

MELMap MELMutableMapToMELMap(MELMutableMap self);

#endif /* mutablemap_h */
