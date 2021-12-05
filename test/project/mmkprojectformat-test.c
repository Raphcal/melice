//
//  mmkprojectformat-test.c
//  melice
//
//  Created by Raphaël Calabro on 17/11/2021.
//

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <melice.h>
#include <assert.h>

int main(int argc, char **argv) {
    MELFileManager *fileManager = MELFileManagerGetSharedInstance();
    MELFileManagerInitWithArguments(fileManager, argv);

    char *path = MELFileManagerPathForAsset(fileManager, "HelloVita.mmk");
    MELInputStream inputStream = MELInputStreamOpen(path);
    if (!inputStream.file) {
        fprintf(stderr, "Unable to access file at path: %s\n", path);
        return EXIT_FAILURE;
    }

    MELProjectFormat mmkFormat = {&MELMmkProjectFormatClass, NULL, 0};

    MELProject project;
    if (!mmkFormat.class->openProject(&mmkFormat, &inputStream, &project)) {
        fprintf(stderr, "Unable to open project at path: %s\n", path);
        return EXIT_FAILURE;
    }

    printf("%zu palette(s)\n", project.palettes.count);
    for (size_t index = 0; index < project.palettes.count; index++) {
        printf("- palette %s (%dx%d)\n", project.palettes.memory[index]->name, project.palettes.memory[index]->tileSize.width, project.palettes.memory[index]->tileSize.height);
    }
    MELMapGroup mapGroup = project.mapGroups.memory[0];
    printf("%zu map(s)\n", mapGroup.maps.count);
    for (size_t index = 0; index < mapGroup.maps.count; index++) {
        MELMutableMap map = mapGroup.maps.memory[index];
        printf("- map %s (%dx%d)\n", map.name, map.size.width, map.size.height);
        printf("  - %zu layer(s)\n", map.layers.count);
        for (size_t layerIndex = 0; layerIndex < map.layers.count; layerIndex++) {
            MELLayer layer = map.layers.memory[layerIndex];
            printf("  - layer %s (%dx%d), %zu sprite(s)\n", layer.name, layer.size.width, layer.size.height, layer.sprites.count);
        }
    }
    printf("%zu sprite(s)\n", mapGroup.sprites.count);
    for (size_t index = 0; index < mapGroup.sprites.count; index++) {
        MELSpriteDefinition sprite = mapGroup.sprites.memory[index];
        printf("- sprite %s, %zu animation(s)\n", sprite.name, sprite.animations.count);
        for (size_t animationIndex = 0; animationIndex < sprite.animations.count; animationIndex++) {
            MELAnimationDefinition animation = sprite.animations.memory[animationIndex];
            printf("  - animation %s, %d frame(s)\n", animation.name, animation.frameCount);
        }
    }

    assert(project.mapGroups.count == 1);
    assert(project.mapGroups.memory[0].maps.count == 2);

    MELMutableMap firstMap = project.mapGroups.memory[0].maps.memory[0];
    assert(MELColorToUInt32Color(firstMap.backgroundColor) == (uint32_t)0xff0099cc);
    assert(firstMap.layers.count == 2);

    assert(strcmp("Background", firstMap.layers.memory[0].name) == 0);
    assert(strcmp("HelloVita", firstMap.layers.memory[1].name) == 0);

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

    return EXIT_SUCCESS;
}
