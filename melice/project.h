//
//  project.h
//  melice
//
//  Created by Raphaël Calabro on 29/10/2021.
//

#ifndef project_h
#define project_h

#include "melstd.h"

#include "list.h"
#include "palette.h"
#include "mapgroup.h"

typedef struct {
    MELList(MELPaletteRef) palettes;
    MELList(MELMapGroup) mapGroups;
} MELProject;

void MELProjectDeinit(MELProject * _Nonnull self);

#endif /* project_h */
