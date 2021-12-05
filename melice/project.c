//
//  project.c
//  melice
//
//  Created by Raphaël Calabro on 02/11/2021.
//

#include "project.h"

void MELProjectDeinit(MELProject * _Nonnull self) {
    MELPaletteRefListDeinitWithDeinitFunction(&self->palettes, &MELPaletteRefDeinit);
    MELMapGroupListDeinitWithDeinitFunction(&self->mapGroups, &MELMapGroupDeinit);
}
