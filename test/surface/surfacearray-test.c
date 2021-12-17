//
//  surfacearray-test.c
//  melice-test
//
//  Created by Raphaël Calabro on 17/12/2021.
//

#include <stdlib.h>
#include <melice.h>
#include <assert.h>

void testSurfaceArrayAllocDealloc(void) {
    MELSurfaceArray array = MELSurfaceArrayMakeWithInitialCapacity(256);
    assert(array.count == 0);

    assert(array.vertex.memory != NULL);
    assert(array.vertex.count == 0);
    assert(array.vertex.capacity == 256);

    assert(array.texture.memory != NULL);
    assert(array.texture.count == 0);
    assert(array.texture.capacity == 256);

    assert(array.color.memory != NULL);
    assert(array.color.count == 0);
    assert(array.color.capacity == 256);

    assert(array.index.memory != NULL);
    assert(array.index.count == 0);
    assert(array.index.capacity == 256);

    MELSurfaceArrayAppendColoredQuad(&array, MELRectangleMake(16, 24, 32, 32), MELUInt8ColorMake(64, 96, 128, 255));

    assert(array.count > 0);
    assert(array.vertex.count == array.count * MELCoordinatesByVertex);
    assert(array.color.count == array.count * MELCoordinatesByColor);

    MELSurfaceArrayDeinit(&array);
}

int main(int argc, char **argv) {
    testSurfaceArrayAllocDealloc();
    return EXIT_SUCCESS;
}
