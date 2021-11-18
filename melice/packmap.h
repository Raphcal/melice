//
//  packmap.h
//  melice
//
//  Created by Raphaël Calabro on 18/11/2021.
//

#ifndef packmap_h
#define packmap_h

#include "melstd.h"

#include "point.h"
#include "size.h"
#include "list.h"
#include "color.h"
#include "palette.h"
#include "spritedefinition.h"

typedef struct melpackmap MELPackMap;

typedef struct {
    void * _Nonnull value;
    uint32_t * _Nonnull pixels;
    MELIntSize size;
    MELIntPoint offset;
} MELPackMapElement;

MELListDefine(MELPackMapElement);

typedef struct {
    int32_t x;
    MELPackMapElement element;
} MELPackMapCell;

MELListDefine(MELPackMapCell);

typedef struct {
    MELPackMap * _Nonnull parent;
    MELIntRectangle frame;
    MELPackMapCellList cells;
} MELPackMapRow;

MELListDefine(MELPackMapRow);

typedef struct melpackmap {
    MELIntSize size;
    MELPackMapElementList elements;
    MELIntPointDictionary origins;
    MELPackMapRowList rows;
    int takenHeight;
} MELPackMap;

MELPackMap MELPackMapMakeWithElements(MELPackMapElementList elements);

MELPackMapElement MELPackMapElementMakeWithPaletteTile(MELPaletteRef palette, unsigned int tileIndex);

void MELPackMapElementListPushPalette(MELPackMapElementList * _Nonnull self, MELPaletteRef palette);
void MELPackMapElementListPushSpriteDefinition(MELPackMapElementList * _Nonnull self, MELSpriteDefinition spriteDefinition);

MELIntPoint MELIntPointDictionaryPutElementOrigin(MELIntPointDictionary * _Nonnull self, MELPackMapElement element, MELIntPoint origin);
MELIntPoint MELIntPointDictionaryGetElementOrigin(MELIntPointDictionary self, MELPackMapElement element);

#endif /* packmap_h */
