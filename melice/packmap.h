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
#include "pointerintrectangletable.h"

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
    MELPointerMELIntRectangleTable origins;
    MELPackMapRowList rows;
    int takenHeight;
} MELPackMap;

MELPackMap MELPackMapMakeWithElements(MELPackMapElementList elements);
void MELPackMapDeinit(MELPackMap * _Nonnull self);

MELIntRectangle MELPackMapFrameForPaletteTile(MELPackMap self, MELPalette * _Nonnull palette, unsigned int tileIndex);

MELPackMapElement MELPackMapElementMakeWithPaletteTile(MELPaletteRef palette, unsigned int tileIndex);
void MELPackMapElementDeinit(MELPackMapElement * _Nonnull self);

void MELPackMapElementListPushPalette(MELPackMapElementList * _Nonnull self, MELPaletteRef palette);
void MELPackMapElementListPushOneFrameOfEachSpriteDefinitionFromList(MELPackMapElementList * _Nonnull self, MELSpriteDefinitionList spriteDeinitionList);
void MELPackMapElementListPushSpriteDefinitionList(MELPackMapElementList * _Nonnull self, MELSpriteDefinitionList spriteDefinitionList);

void MELPackMapRowDeinit(MELPackMapRow * _Nonnull self);

#endif /* packmap_h */
