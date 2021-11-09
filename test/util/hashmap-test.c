//
//  hashmap-test.c
//  melice
//
//  Created by Raphaël Calabro on 09/11/2021.
//

#include <stdio.h>
#include <melice.h>
#include <assert.h>

void debugEntries(MELHashMap hashMap) {
    printf("HashMap, %zu entries, buckets: %zu used / %zu total\n", hashMap.count, hashMap.buckets.count, hashMap.buckets.capacity);
    for (int i = 0; i < hashMap.buckets.capacity; i++) {
        MELHashMapBucket bucket = hashMap.buckets.memory[i];
        printf("  - Bucket %d: %zu entries\n", i, bucket.entries.count);
        for (int j = 0; j < bucket.entries.count; j++) {
            MELHashMapEntry entry = bucket.entries.memory[j];
            printf("    - %llu: %s => %s\n", entry.hash, (char *)entry.key, (char *)entry.value);
        }
    }
}

void testPutCount(void) {
    MELHashMap hashMap = MELHashMapMakeWithStringKeys();
    assert(hashMap.count == 0);

    MELHashMapPut(&hashMap, "lastName", "The Hedgehog");
    assert(hashMap.count == 1);

    MELHashMapPut(&hashMap, "firstName", "Sonic");
    assert(hashMap.count == 2);

    MELHashMapPut(&hashMap, "color", "Blue");
    MELHashMapPut(&hashMap, "speed", "Fast");
    MELHashMapPut(&hashMap, "favoriteFood", "Chilly Hot Dog");
    assert(hashMap.count == 5);

    MELHashMapPut(&hashMap, "firstName", "Super Sonic");
    MELHashMapPut(&hashMap, "color", "Yellow");
    assert(hashMap.count == 5);

    MELHashMapDeinit(&hashMap);
    assert(hashMap.count == 0);
}

void testPutGet(void) {
    MELHashMap hashMap = MELHashMapMakeWithStringKeys();
    assert(MELHashMapGet(&hashMap, "firstName") == NULL);

    MELHashMapPut(&hashMap, "lastName", "The Hedgehog");
    MELHashMapPut(&hashMap, "firstName", "Sonic");
    MELHashMapPut(&hashMap, "color", "Blue");
    MELHashMapPut(&hashMap, "speed", "Fast");
    MELHashMapPut(&hashMap, "favoriteFood", "Chilly Hot Dog");

    assert(strcmp(MELHashMapGet(&hashMap, "firstName"), "Sonic") == 0);
    assert(strcmp(MELHashMapGet(&hashMap, "lastName"), "The Hedgehog") == 0);
    assert(strcmp(MELHashMapGet(&hashMap, "color"), "Blue") == 0);
    assert(strcmp(MELHashMapGet(&hashMap, "speed"), "Fast") == 0);
    assert(strcmp(MELHashMapGet(&hashMap, "favoriteFood"), "Chilly Hot Dog") == 0);

    assert(MELHashMapGet(&hashMap, "notAValidKey") == NULL);

    MELHashMapDeinit(&hashMap);
    assert(MELHashMapGet(&hashMap, "firstName") == NULL);
    assert(MELHashMapGet(&hashMap, "lastName") == NULL);
    assert(MELHashMapGet(&hashMap, "color") == NULL);
    assert(MELHashMapGet(&hashMap, "speed") == NULL);
    assert(MELHashMapGet(&hashMap, "favoriteFood") == NULL);
}

int main(int argc, char **argv) {
    testPutCount();
    testPutGet();
    return EXIT_SUCCESS;
}
