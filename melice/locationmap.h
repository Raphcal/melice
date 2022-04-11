//
//  locationmap.h
//  melice
//
//  Created by Raphaël Calabro on 03/01/2022.
//

#ifndef locationmap_h
#define locationmap_h

#include "melstd.h"

#include "list.h"
#include "primitives.h"
#include "size.h"
#include "rectangle.h"

typedef struct {
    MELRef value;
    MELIntRectangle frame;
} MELLocationMapEntry;

MELListDefine(MELLocationMapEntry);

typedef struct {
    MELLocationMapEntryList entries;
} MELLocationMapCell;

MELListDefine(MELLocationMapCell);

typedef struct {
    MELIntSize size;
    MELIntSize cellSize;
    MELLocationMapCellList cells;
} MELLocationMap;

extern const MELLocationMap MELLocationMapEmpty;

MELLocationMap MELLocationMapMakeWithSize(MELIntSize size);

void MELLocationMapPutObjectWithFrame(MELLocationMap * _Nonnull self, MELRefList object, MELIntRectangle frame);
MELRefList MELLocationMapEntriesAtPoint(MELLocationMap self, MELIntPoint point);
MELRefList MELLocationMapEntriesInRect(MELLocationMap self, MELIntRectangle rectangle);

#endif /* locationmap_h */
