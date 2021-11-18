//
//  packmap.c
//  melice
//
//  Created by Raphaël Calabro on 18/11/2021.
//

#include "packmap.h"

MELListImplement(MELPackMapElement);
MELListImplement(MELPackMapCell);
MELListImplement(MELPackMapRow);

MELPackMap MELPackMapMakeWithElements(MELPackMapElementList elements);
int32_t MELPackMapRemainingHeight(MELPackMap * _Nonnull self);
void MELPackMapAddElement(MELPackMap * _Nonnull self, MELPackMapElement element);
void MELPackMapGrow(MELPackMap * _Nonnull self);

MELPackMapRow MELPackMapRowMake(MELPackMap * _Nonnull parent, MELIntPoint origin, int32_t height);
MELPackMapRow MELPackMapRowMakeWithFirstElement(MELPackMap * _Nonnull parent, MELPackMapElement first, MELIntPoint origin);
int32_t MELPackMapRowRemainingWidth(MELPackMapRow * _Nonnull self);
void MELPackMapRowAddElement(MELPackMapRow * _Nonnull self, MELPackMapElement element);
MELBoolean MELPackMapRowIsLastElementLargerThanHeight(MELPackMapRow * _Nonnull self, int32_t height);

MELPackMapCell MELPackMapCellMake(int32_t x, MELPackMapElement element);

int MELPackMapElementCompare(const MELPackMapElement *lhs, const  MELPackMapElement *rhs);

#pragma mark - MELPackMap

MELPackMap MELPackMapMakeWithElements(MELPackMapElementList elements) {
    MELPackMap self = {MELIntSizeMake(64, 64), elements, MELIntPointDictionaryEmpty, MELPackMapRowListEmpty, 0};
    qsort(elements.memory, elements.count, sizeof(MELPackMapElement), (int(*)(const void *, const void *)) &MELPackMapElementCompare);
    for (size_t index = 0; index < elements.count; index++) {
        MELPackMapAddElement(&self, elements.memory[index]);
    }
    return self;
}

int32_t MELPackMapRemainingHeight(MELPackMap * _Nonnull self) {
    return self->size.height - self->takenHeight;
}

void MELPackMapAddElement(MELPackMap * _Nonnull self, MELPackMapElement element) {
    for (;;) {
        for (size_t index = 0; index < self->rows.count; index++) {
            MELPackMapRow *row = self->rows.memory + index;
            MELIntRectangle frame = row->frame;
            if (frame.size.height >= element.size.height && MELPackMapRowRemainingWidth(row) >= element.size.width) {
                MELPackMapRowAddElement(row, element);

                MELIntPoint origin = MELIntPointMake(frame.origin.x + row->cells.memory[row->cells.count - 1].x, frame.origin.y);
                MELIntPointDictionaryPutElementOrigin(&self->origins, element, origin);

                if (MELPackMapRowIsLastElementLargerThanHeight(row, element.size.height)) {
                    row->frame.size.height = element.size.height;
                    MELPackMapRowListInsert(&self->rows, index + 1, MELPackMapRowMake(self, MELIntPointMake(origin.x, origin.y + element.size.height), frame.size.height - element.size.height));
                }
                return;
            }
        }
        if (MELPackMapRemainingHeight(self) >= element.size.height) {
            MELIntPoint origin = MELIntPointMake(0, self->takenHeight);
            MELPackMapRowListPush(&self->rows, MELPackMapRowMakeWithFirstElement(self, element, origin));
            self->takenHeight += element.size.height;

            MELIntPointDictionaryPutElementOrigin(&self->origins, element, origin);
            return;
        } else {
            MELPackMapGrow(self);
        }
    }
}

void MELPackMapGrow(MELPackMap * _Nonnull self) {
    MELIntSize size = self->size;
    size.width *= 2;
    size.height *= 2;
    self->size = size;
}

#pragma mark - MELPackMapRow

MELPackMapRow MELPackMapRowMake(MELPackMap * _Nonnull parent, MELIntPoint origin, int32_t height) {
    return (MELPackMapRow) {parent, MELIntRectangleMakeWithOriginAndSize(origin, MELIntSizeMake(0, height)), MELPackMapCellListEmpty};
}

MELPackMapRow MELPackMapRowMakeWithFirstElement(MELPackMap * _Nonnull parent, MELPackMapElement first, MELIntPoint origin) {
    MELPackMapCellList cells = MELPackMapCellListMake();
    MELPackMapCellListPush(&cells, MELPackMapCellMake(0, first));
    return (MELPackMapRow) {parent, MELIntRectangleMakeWithOriginAndSize(origin, first.size), cells};
}

int32_t MELPackMapRowRemainingWidth(MELPackMapRow * _Nonnull self) {
    return self->parent->size.width - self->frame.size.width - self->frame.origin.x;
}

void MELPackMapRowAddElement(MELPackMapRow * _Nonnull self, MELPackMapElement element) {
    MELPackMapCell cell = {self->frame.size.width, element};
    MELPackMapCellListPush(&self->cells, cell);
    self->frame.size.width += element.size.width;
}

MELBoolean MELPackMapRowIsLastElementLargerThanHeight(MELPackMapRow * _Nonnull self, int32_t height) {
    return (self->cells.count == 0 && self->frame.size.height > height)
    || (self->cells.count > 0 && self->cells.memory[self->cells.count - 1].element.size.height > height);
}

#pragma mark - MELPackMapCell

MELPackMapCell MELPackMapCellMake(int32_t x, MELPackMapElement element) {
    return (MELPackMapCell) {x, element};
}

#pragma mark - MELPackMapElement

#define PADDING 1

const MELPackMapElement MELPackMapElementEmpty = {NULL, NULL, {0, 0}, {0, 0}};

MELPackMapElement MELPackMapElementMake(void * _Nonnull value, uint32_t * _Nonnull pixels, MELIntSize size, MELIntPoint offset) {
    return (MELPackMapElement) {value, pixels, size, offset};
}

MELPackMapElement MELPackMapElementMakeWithPaletteTile(MELPaletteRef palette, unsigned int tileIndex) {
    const MELIntSize tileSize = palette->tileSize;
    const MELIntSize size = MELIntSizeMake(tileSize.width + PADDING + PADDING, tileSize.height + PADDING + PADDING);
    const size_t pixelCount = size.width * size.height;
    const MELIntPoint offset = MELIntPointMake(PADDING, PADDING);

    MELUInt32Color *pixels = calloc(sizeof(MELUInt32Color), pixelCount);
    palette->class->paintTileToBuffer(palette, tileIndex, offset, pixels, size);

    MELBoolean isEmpty = true;
    for (unsigned int index = 0; index < pixelCount; index++) {
        if (pixels[index]) {
            isEmpty = false;
            break;
        }
    }
    if (isEmpty) {
        free(pixels);
        return MELPackMapElementEmpty;
    }

    const uintptr_t imageTopStart = PADDING + PADDING * size.width;
    const uintptr_t imageBottomStart = PADDING + (PADDING + tileSize.height - 1) * size.width;
    const uintptr_t borderTopStart = imageTopStart - size.width;
    const uintptr_t borderBottomStart = imageBottomStart + size.width;
    const size_t rowSize = sizeof(MELUInt32Color) * tileSize.width;

    memcpy(pixels + borderTopStart, pixels + imageTopStart, rowSize);
    const MELUInt32Color *end = pixels + borderBottomStart;
    for (MELUInt32Color *pointer = pixels + imageTopStart - 1; pointer < end; pointer += size.width) {
        pointer[0] = pointer[1];
        pointer[tileSize.width + 1] = pointer[tileSize.width];
    }
    memcpy(pixels + borderBottomStart, pixels + imageBottomStart, rowSize);

    return MELPackMapElementMake(palette->class->tileAtIndex(palette, tileIndex), pixels, size, offset);
}

int MELPackMapElementCompare(const MELPackMapElement *lhs, const  MELPackMapElement *rhs) {
    return rhs->size.height - lhs->size.height;
}

#pragma mark - MELPackMapElementList

void MELPackMapElementListPushPalette(MELPackMapElementList * _Nonnull self, MELPaletteRef palette) {
    const size_t count = palette->count;
    for (unsigned int index = 0; index < count; index++) {
        MELPackMapElement element = MELPackMapElementMakeWithPaletteTile(palette, index);
        if (element.value != NULL) {
            MELPackMapElementListPush(self, element);
        }
    }
}

void MELPackMapElementListPushSpriteDefinition(MELPackMapElementList * _Nonnull self, MELSpriteDefinition spriteDefinition) {
    // TODO: Implement MELPackMapElementListPushSpriteDefinition.
}

#pragma mark - MELIntPointDictionary

MELIntPoint MELIntPointDictionaryPutElementOrigin(MELIntPointDictionary * _Nonnull self, MELPackMapElement element, MELIntPoint origin) {
    char key[255];
    sprintf(key, "%lx", (uintptr_t) element.value);
    return MELIntPointDictionaryPut(self, key, origin);
}

MELIntPoint MELIntPointDictionaryGetElementOrigin(MELIntPointDictionary self, MELPackMapElement element) {
    char key[255];
    sprintf(key, "%lx", (uintptr_t) element.value);
    return MELIntPointDictionaryGet(self, key);
}
