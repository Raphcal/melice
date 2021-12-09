//
//  project.c
//  melice
//
//  Created by Raphaël Calabro on 02/11/2021.
//

#include "project.h"

#include "colorpalette.h"
#include "imagepalette.h"
#include "size.h"

#define DEFAULT_MAP_SIZE ((MELIntSize){20,15})

MELProject MELProjectMakeWithEmptyMap(void) {
    MELProject self = {
        MELPaletteRefListMakeWithInitialCapacity(2),
        MELMapGroupListMakeWithInitialCapacity(1)
    };

    // Default color palette.
    MELColorPalette *defaultColorPalette = malloc(sizeof(MELColorPalette));
    *defaultColorPalette = MELColorPaletteMakeDefault();
    MELPaletteRefListPush(&self.palettes, &defaultColorPalette->super);

    // Empty image palette.
    MELImagePalette *imagePalette = malloc(sizeof(MELImagePalette));
    *imagePalette = MELImagePaletteMakeWithColorPalette((MELColorPalette *) self.palettes.memory[0]);
    MELPaletteRefListPush(&self.palettes, &imagePalette->super);

    // Empty map.
    MELMutableMap map = MELMutableMapMakeWithSizeAndPalette(DEFAULT_MAP_SIZE, self.palettes.memory[1]);

    // Map groups.
    MELMapGroup mapGroup = MELMapGroupEmpty;
    MELMutableMapListPush(&mapGroup.maps, map);
    MELMapGroupListPush(&self.mapGroups, mapGroup);

    return self;
}

void MELProjectDeinit(MELProject * _Nonnull self) {
    MELPaletteRefListDeinitWithDeinitFunction(&self->palettes, &MELPaletteRefDeinit);
    MELMapGroupListDeinitWithDeinitFunction(&self->mapGroups, &MELMapGroupDeinit);
}
