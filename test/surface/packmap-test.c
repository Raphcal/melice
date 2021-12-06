//
//  packmap-test.c
//  melice
//
//  Created by Raphaël Calabro on 08/12/2021.
//

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <melice.h>
#include <assert.h>

void testPackMap(void) {
    MELFileManager *fileManager = MELFileManagerGetSharedInstance();
    MELInputStream inputStream = MELFileManagerOpenAsset(fileManager, "HelloVita.mmk");
    assert(inputStream.file);

    MELProjectFormat mmkFormat = MELMmkProjectFormat;

    MELProject project;
    MELBoolean openSuccess = mmkFormat.class->openProject(&mmkFormat, &inputStream, &project);
    assert(openSuccess);

    MELPaletteRef palette = project.palettes.memory[0];
    MELPackMapElementList elements = MELPackMapElementListMakeWithInitialCapacity(palette->count);
    MELPackMapElementListPushPalette(&elements, palette);
    assert(elements.count == 11);

    MELPackMap packMap = MELPackMapMakeWithElements(elements);

    MELTexture texture = MELTextureMakeWithPackMap(packMap);
    char *bmpPath = MELFileManagerPathForAsset(fileManager, "palette0.bmp");
    MELBitmapSave(bmpPath, texture.pixels, texture.size);
    remove(bmpPath);
    free(bmpPath);
    MELTextureDeinit(&texture);
    MELProjectDeinit(&project);
}

int main(int argc, char **argv) {
    MELFileManagerInitWithArguments(MELFileManagerGetSharedInstance(), argv);

    testPackMap();

    return EXIT_SUCCESS;
}
