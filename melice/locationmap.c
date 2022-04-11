//
//  locationmap.c
//  melice
//
//  Created by Raphaël Calabro on 03/01/2022.
//

#include "locationmap.h"

const MELLocationMap MELLocationMapEmpty = {};

MELLocationMap MELLocationMapMakeWithSize(MELIntSize size) {
    return MELLocationMapEmpty;
}

void MELLocationMapPutObjectWithFrame(MELLocationMap * _Nonnull self, MELRefList object, MELIntRectangle frame) {
    // TODO: Implémenter
}

MELRefList MELLocationMapEntriesAtPoint(MELLocationMap self, MELIntPoint point) {
    return MELRefListEmpty;
}

MELRefList MELLocationMapEntriesInRect(MELLocationMap self, MELIntRectangle rectangle) {
    return MELRefListEmpty;
}
