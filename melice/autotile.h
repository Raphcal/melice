//
//  autotile.h
//  melice
//
//  Created by Raphaël Calabro on 28/05/2026.
//

#ifndef autotile_h
#define autotile_h

#include "melstd.h"

#include "color.h"
#include "list.h"

#define MELAutoTilePatternSize 9

typedef struct melimagepalette MELImagePalette;
typedef struct melautotilegroup MELAutoTileGroup;
typedef struct melautotilepattern MELAutoTilePattern;
typedef struct melautotileoutput MELAutoTileOutput;

typedef struct melautotileoutput {
    MELAutoTilePattern * _Nullable pattern;
    char * _Nullable name;
    uint32_t width;
    uint32_t height;
    int32_t tiles[MELAutoTilePatternSize];
    int32_t weight;
} MELAutoTileOutput;

MELListDefine(MELAutoTileOutput);

typedef struct melautotilepattern {
    MELAutoTileGroup * _Nullable group;
    char * _Nullable name;
    uint32_t width;
    uint32_t height;
    int32_t pattern[MELAutoTilePatternSize];
    MELAutoTileOutputList outputs;
} MELAutoTilePattern;

MELListDefine(MELAutoTilePattern);

typedef struct melautotilegroup {
    MELImagePalette * _Nullable palette;
    char * _Nullable name;
    MELColor color;
    int16_t layer;
    MELAutoTilePatternList patterns;
} MELAutoTileGroup;

MELListDefine(MELAutoTileGroup);

#endif /* autotile_h */
