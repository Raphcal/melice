//
//  dictionary-test.c
//  melice
//
//  Created by Raphaël Calabro on 09/11/2021.
//

#include <stdio.h>
#include <melice.h>
#include <assert.h>

MELDictionaryDefine(MELPoint);
MELDictionaryImplement(MELPoint, MELPointZero);

void debugEntries(MELPointDictionary dictionary) {
    printf("MELPointDictionary, %zu entries, buckets: %zu used / %zu total\n", dictionary.count, dictionary.buckets.count, dictionary.buckets.capacity);
    for (int i = 0; i < dictionary.buckets.capacity; i++) {
        MELPointDictionaryBucket bucket = dictionary.buckets.memory[i];
        printf("  - Bucket %d: %zu entries\n", i, bucket.entries.count);
        for (int j = 0; j < bucket.entries.count; j++) {
            MELPointDictionaryEntry entry = bucket.entries.memory[j];
            printf("    - %llu: %s => %fx%f\n", entry.hash, (char *)entry.key, entry.value.x, entry.value.y);
        }
    }
}

void debugList(MELPointDictionaryEntryList list) {
    printf("MELPointDictionaryEntryList\n");
    for (unsigned int index = 0; index < list.count; index++) {
        MELPointDictionaryEntry entry = list.memory[index];
        printf("    - %llu: %s => %fx%f\n", entry.hash, (char *)entry.key, entry.value.x, entry.value.y);
    }
}

void testPutCount(void) {
    MELPointDictionary dictionary = MELPointDictionaryEmpty;
    assert(dictionary.count == 0);

    MELPointDictionaryPut(&dictionary, "center", MELPointMake(16, 16));
    assert(dictionary.count == 1);

    MELPointDictionaryPut(&dictionary, "topLeft", MELPointMake(0, 0));
    assert(dictionary.count == 2);

    MELPointDictionaryPut(&dictionary, "topRight", MELPointMake(32, 0));
    MELPointDictionaryPut(&dictionary, "bottomRight", MELPointMake(32, 32));
    MELPointDictionaryPut(&dictionary, "bottomLeft", MELPointMake(0, 32));
    assert(dictionary.count == 5);

    MELPointDictionaryPut(&dictionary, "center", MELPointMake(24, 16));
    MELPointDictionaryPut(&dictionary, "topRight", MELPointMake(48, 0));
    MELPointDictionaryPut(&dictionary, "bottomRight", MELPointMake(48, 32));
    assert(dictionary.count == 5);

    MELPointDictionaryDeinit(&dictionary);
    assert(dictionary.count == 0);
}

void testPutGet(void) {
    MELPointDictionary dictionary = MELPointDictionaryEmpty;
    assert(MELPointEquals(MELPointDictionaryGet(dictionary, "center"), MELPointZero));

    MELPointDictionaryPut(&dictionary, "center", MELPointMake(16, 16));
    MELPointDictionaryPut(&dictionary, "topLeft", MELPointMake(0, 0));
    MELPointDictionaryPut(&dictionary, "topRight", MELPointMake(32, 0));
    MELPointDictionaryPut(&dictionary, "bottomRight", MELPointMake(32, 32));
    MELPointDictionaryPut(&dictionary, "bottomLeft", MELPointMake(0, 32));

    assert(MELPointEquals(MELPointDictionaryGet(dictionary, "center"), MELPointMake(16, 16)));
    assert(MELPointEquals(MELPointDictionaryGet(dictionary, "topLeft"), MELPointZero));
    assert(MELPointEquals(MELPointDictionaryGet(dictionary, "topRight"), MELPointMake(32, 0)));
    assert(MELPointEquals(MELPointDictionaryGet(dictionary, "bottomRight"), MELPointMake(32, 32)));
    assert(MELPointEquals(MELPointDictionaryGet(dictionary, "bottomLeft"), MELPointMake(0, 32)));

    assert(MELPointEquals(MELPointDictionaryGet(dictionary, "notAValidKey"), MELPointZero));

    MELPointDictionaryDeinit(&dictionary);
    assert(MELPointEquals(MELPointDictionaryGet(dictionary, "center"), MELPointZero));
    assert(MELPointEquals(MELPointDictionaryGet(dictionary, "topLeft"), MELPointZero));
    assert(MELPointEquals(MELPointDictionaryGet(dictionary, "topRight"), MELPointZero));
    assert(MELPointEquals(MELPointDictionaryGet(dictionary, "bottomRight"), MELPointZero));
    assert(MELPointEquals(MELPointDictionaryGet(dictionary, "bottomLeft"), MELPointZero));
}

void testRemove(void) {
    MELPointDictionary dictionary = MELPointDictionaryEmpty;
    assert(dictionary.count == 0);

    MELPointDictionaryPut(&dictionary, "center", MELPointMake(16, 16));
    MELPointDictionaryPut(&dictionary, "topLeft", MELPointMake(0, 0));
    MELPointDictionaryPut(&dictionary, "topRight", MELPointMake(32, 0));
    MELPointDictionaryPut(&dictionary, "bottomRight", MELPointMake(32, 32));
    MELPointDictionaryPut(&dictionary, "bottomLeft", MELPointMake(0, 32));
    assert(dictionary.count == 5);
    assert(MELPointEquals(MELPointDictionaryGet(dictionary, "center"), MELPointMake(16, 16)));

    MELPointDictionaryRemove(&dictionary, "center");
    assert(dictionary.count == 4);
    assert(MELPointEquals(MELPointDictionaryGet(dictionary, "center"), MELPointZero));

    MELPointDictionaryDeinit(&dictionary);
    assert(dictionary.count == 0);
}

void testEntries(void) {
    MELPointDictionary dictionary = MELPointDictionaryEmpty;
    assert(dictionary.count == 0);

    MELPointDictionaryPut(&dictionary, "center", MELPointMake(16, 16));
    MELPointDictionaryPut(&dictionary, "topRight", MELPointMake(32, 0));
    MELPointDictionaryPut(&dictionary, "bottomLeft", MELPointMake(0, 32));
    assert(dictionary.count == 3);

    MELPointDictionaryEntryList entries = MELPointDictionaryEntries(&dictionary);
    assert(entries.count == 3);

    unsigned int centerCount = 0;
    unsigned int topRightCount = 0;
    unsigned int bottomLeftCount = 0;
    for (unsigned int index = 0; index < entries.count; index++) {
        MELPointDictionaryEntry entry = entries.memory[index];
        if (strcmp("center", entry.key) == 0) {
            centerCount++;
        } else if (strcmp("topRight", entry.key) == 0) {
            topRightCount++;
        }  else if (strcmp("bottomLeft", entry.key) == 0) {
            bottomLeftCount++;
        }
    }
    assert(centerCount == 1);
    assert(topRightCount == 1);
    assert(bottomLeftCount == 1);

    MELPointDictionaryDeinit(&dictionary);
}

void testRehash(void) {
    MELPointDictionary dictionary = MELPointDictionaryEmpty;

    unsigned int changeCount = 0;
    size_t oldCapacity = dictionary.buckets.capacity;
    for (int index = 0; index < 250; index++) {
        char *key = calloc(sizeof(char), 9);
        sprintf(key, "entry%d", index);
        MELPointDictionaryPut(&dictionary, key, MELPointMake(index, index));

        if (oldCapacity != dictionary.buckets.capacity) {
            // Capacity did change from oldCapacity to dictionary.buckets.capacity
            oldCapacity = dictionary.buckets.capacity;
            changeCount++;
        }
    }
    assert(changeCount == 6);
    assert(dictionary.buckets.capacity == 512);

    MELPointDictionaryDeinit(&dictionary);
}

int main(int argc, char **argv) {
    testPutCount();
    testPutGet();
    testRemove();
    testRehash();
    testEntries();
    return EXIT_SUCCESS;
}
