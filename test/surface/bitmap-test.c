//
//  bitmap-test.c
//  melice
//
//  Created by Raphaël Calabro on 22/11/2021.
//

#include <stdlib.h>
#include <unistd.h>
#include <melice.h>
#include <assert.h>

void assertFilesEquals(const char *expectedPath, const char *actualPath);

void testBitmapSave(void) {
    const MELIntSize size = MELIntSizeMake(4, 2);
    const MELUInt8Color bitmap[] = {
        MELUInt8ColorMake(0, 0, 255, 255),
        MELUInt8ColorMake(0, 255, 0, 255),
        MELUInt8ColorMake(255, 0, 0, 255),
        MELUInt8ColorMake(255, 255, 255, 255),
        MELUInt8ColorMake(0, 0, 255, 127),
        MELUInt8ColorMake(0, 255, 0, 127),
        MELUInt8ColorMake(255, 0, 0, 127),
        MELUInt8ColorMake(255, 255, 255, 127)
    };

    MELFileManager *fileManager = MELFileManagerGetSharedInstance();
    char *path = MELFileManagerPathForAsset(fileManager, "actual.bmp");

    MELBitmapSave(path, (MELUInt32Color *) bitmap, size);
    assert (access(path, F_OK) == 0);
    assertFilesEquals("expected.bmp", "actual.bmp");
    assert(remove(path) == 0);
    free(path);
}

int main(int argc, char **argv) {
    MELFileManager *fileManager = MELFileManagerGetSharedInstance();
    MELFileManagerInitWithArguments(fileManager, argv);

    testBitmapSave();
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
