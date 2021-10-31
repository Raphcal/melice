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

typedef struct melpalette MELPalette;

typedef struct {
    MELIntSize size;
    MELColor backgroundColor;
    MELList(MELLayer) layers;
    MELPalette * _Nullable palette;
    uint16_t * _Nullable name;
} MELMutableMap;

MELListDefine(MELMutableMap);

#endif /* mutablemap_h */
