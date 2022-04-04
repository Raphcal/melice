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
#include "melstring.h"

typedef struct {
    MELList(MELPaletteRef) palettes;
    MELMapGroup root;
    MELStringList animationNames;
} MELProject;

MELProject MELProjectMakeWithEmptyMap(void);
MELProject MELProjectMakeWithProject(MELProject other);
void MELProjectDeinit(MELProject * _Nonnull self);

#endif /* project_h */
