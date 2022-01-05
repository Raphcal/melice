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

const char projectFile[] = "HelloVita.mmk";

void assertFilesEquals(const char *expectedPath, const char *actualPath);

void debugProject(MELProject project) {
    printf("%zu palette(s)\n", project.palettes.count);
    for (size_t index = 0; index < project.palettes.count; index++) {
        printf("- palette %s (%dx%d)\n", project.palettes.memory[index]->name, project.palettes.memory[index]->tileSize.width, project.palettes.memory[index]->tileSize.height);
    }
    MELMapGroup mapGroup = project.root;
    printf("%zu map(s)\n", mapGroup.maps.count);
    for (size_t index = 0; index < mapGroup.maps.count; index++) {
        MELMutableMap map = mapGroup.maps.memory[index];
        printf("- map %s (%dx%d)\n", map.name, map.super.size.width, map.super.size.height);
        printf("  - %zu layer(s)\n", map.super.layers.count);
        for (size_t layerIndex = 0; layerIndex < map.super.layers.count; layerIndex++) {
            MELLayer layer = map.super.layers.memory[layerIndex];
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
}

void testOpenProjet(void) {
    MELFileManager *fileManager = MELFileManagerGetSharedInstance();

    MELInputStream inputStream = MELFileManagerOpenAsset(fileManager, projectFile);
    assert(inputStream.file);

    MELProjectFormat mmkFormat = MELMmkProjectFormat;

    MELProject project;
    MELBoolean openSuccess = mmkFormat.class->openProject(&mmkFormat, &inputStream, &project);
    assert(openSuccess);

    assert(project.root.maps.count == 2);

    MELMutableMap firstMap = project.root.maps.memory[0];
    assert(MELColorToUInt32Color(firstMap.super.backgroundColor) == (uint32_t)0xff0099cc);
    assert(firstMap.super.layers.count == 2);

    assert(strcmp("Background", firstMap.super.layers.memory[0].name) == 0);
    assert(strcmp("HelloVita", firstMap.super.layers.memory[1].name) == 0);

    MELProjectDeinit(&project);
}

void testSaveProject(void) {
    MELFileManager *fileManager = MELFileManagerGetSharedInstance();

    MELInputStream inputStream = MELFileManagerOpenAsset(fileManager, projectFile);
    assert(inputStream.file);

    MELProjectFormat mmkFormat = MELMmkProjectFormat;

    MELProject project;
    MELBoolean openSuccess = mmkFormat.class->openProject(&mmkFormat, &inputStream, &project);
    assert(openSuccess);

    char *outputPath = MELFileManagerPathForAsset(fileManager, "HelloVita-out.mmk");
    MELBoolean saveSuccess = mmkFormat.class->saveProjectAtPath(&mmkFormat, project, outputPath);
    assert(saveSuccess);

    assertFilesEquals("HelloVita.mmk", "HelloVita-out.mmk");

    MELProjectDeinit(&project);
}

int main(int argc, char **argv) {
    MELFileManagerInitWithArguments(MELFileManagerGetSharedInstance(), argv);

    testOpenProjet();
    testSaveProject();

    return EXIT_SUCCESS;
}

uint8_t *bytesForAsset(const char *asset, size_t *outCount) {
    MELFileManager *fileManager = MELFileManagerGetSharedInstance();
    char *path = MELFileManagerPathForAsset(fileManager, asset);

    FILE *file = fopen(path, "rb");
    assert(file != NULL);

    fseek(file, 0, SEEK_END);
    size_t count = ftell(file);
    *outCount = count;
    uint8_t *bytes = malloc(sizeof(uint8_t) * count);
    fseek(file, 0, SEEK_SET);
    assert(fread(bytes, sizeof(uint8_t), count, file) == count);

    fclose(file);
    free(path);

    return bytes;
}

void assertFilesEquals(const char *expectedPath, const char *actualPath) {
    size_t expectedCount;
    uint8_t *expected = bytesForAsset(expectedPath, &expectedCount);

    size_t actualCount;
    uint8_t *actual = bytesForAsset(actualPath, &actualCount);

    if (actualCount != expectedCount) {
        fprintf(stderr, "actualCount: %zu != expectedCount: %zu\n", actualCount, expectedCount);
    }
    size_t count = actualCount < expectedCount ? actualCount : expectedCount;
    for (size_t index = 0; index < count; index++) {
        if (actual[index] != expected[index]) {
            fprintf(stderr, "actual[%zu]: %x != expected[%zu]: %x\n", index, actual[index], index, expected[index]);
            const size_t from = index > 8 ? index - 8 : 0;
            const size_t toActual = index + 8 > actualCount ? actualCount : index + 8;
            const size_t toExpected = index + 8 > expectedCount ? expectedCount : index + 8;
            fprintf(stderr, "expected:");
            for (size_t i = from; i < toExpected; i++) {
                fprintf(stderr, " %02x", expected[i]);
            }
            fprintf(stderr, "\n");
            fprintf(stderr, "but was: ");
            for (size_t i = from; i < toActual; i++) {
                fprintf(stderr, " %02x", actual[i]);
            }
            fprintf(stderr, "\n");
        }
        assert(actual[index] == expected[index]);
    }
    free(expected);
    free(actual);
}
