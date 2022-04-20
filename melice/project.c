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

static const unsigned int defaultAnimationNameCount = 15;

static const char defaultAnimationNames[defaultAnimationNameCount][10] = {
    "stand", "walk", "run", "skid", "jump", "fall", "shaky", "bounce", "duck", "raise", "appear", "disappear", "attack", "hurt", "die"
};

MELProject MELProjectMake(void) {
    MELProject self = {
        MELPaletteRefListEmpty,
        MELMapGroupEmpty,
        MELStringListMakeWithInitialCapacity(defaultAnimationNameCount),
        MELStringDictionaryEmpty
    };

    // Animation names.
    for (int index = 0; index < defaultAnimationNameCount; index++) {
        MELStringListPush(&self.animationNames, MELStringCopy(defaultAnimationNames[index]));
    }
    return self;
}

MELProject MELProjectMakeWithEmptyMap(void) {
    MELProject self = {
        MELPaletteRefListMakeWithInitialCapacity(2),
        MELMapGroupEmpty,
        MELStringListMakeWithInitialCapacity(defaultAnimationNameCount),
        MELStringDictionaryEmpty
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
    MELMutableMapListPush(&self.root.maps, map);

    // Animation names.
    for (int index = 0; index < defaultAnimationNameCount; index++) {
        MELStringListPush(&self.animationNames, MELStringCopy((const MELString) defaultAnimationNames[index]));
    }

    return self;
}

MELProject MELProjectMakeWithProject(MELProject other) {
    MELProject self = {
        MELPaletteRefListMakeWithListAndCopyFunction(other.palettes, &MELPaletteRefMakeWithPaletteRef),
        MELMapGroupMakeWithMapGroup(other.root),
        MELStringListMakeWithListAndCopyFunction(other.animationNames, (MELString (* _Nonnull)(MELString)) &MELStringCopy),
        MELStringDictionaryMakeWithDictionary(other.scripts)
    };
    return self;
}

void MELProjectDeinit(MELProject * _Nonnull self) {
    MELPaletteRefListDeinitWithDeinitFunction(&self->palettes, &MELPaletteRefDeinit);
    MELMapGroupDeinit(&self->root);
    MELStringListDeinitWithDeinitFunction(&self->animationNames, &MELStringDeinit);
    MELStringDictionaryDeinit(&self->scripts);
}
