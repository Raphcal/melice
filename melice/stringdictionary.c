//
//  stringdictionary.c
//  melice
//
//  Created by Raphaël Calabro on 20/04/2022.
//

#include "stringdictionary.h"

#include <string.h>
#include "melstring.h"

#define LOAD_FACTOR 0.75
#define DEFAULT_BUCKET_COUNT 16

MELListImplement(MELStringDictionaryEntry);
MELListImplement(MELStringDictionaryBucket);

const MELStringDictionary MELStringDictionaryEmpty = {};

MELStringDictionaryEntry MELStringDictionaryEntryMakeWithEntry(MELStringDictionaryEntry other) {
    MELStringDictionaryEntry self;
    self.hash = other.hash;
    self.key = strdup(other.key);
    self.value = MELStringCopy(other.value);
    return self;
}

MELStringDictionaryBucket MELStringDictionaryBucketMakeWithBucket(MELStringDictionaryBucket other) {
    MELStringDictionaryBucket self;
    self.entries = MELStringDictionaryEntryListMakeWithListAndCopyFunction(other.entries, &MELStringDictionaryEntryMakeWithEntry);
    return self;
}

MELStringDictionary MELStringDictionaryMakeWithDictionary(MELStringDictionary other) {
    MELStringDictionary self;
    self.count = other.count;
    self.buckets = MELStringDictionaryBucketListMakeWithListAndCopyFunction(other.buckets, &MELStringDictionaryBucketMakeWithBucket);
    self.keys = MELConstStringListMakeWithInitialCapacity(other.keys.count);
    // Adding copied keys to key list to have the same references.
    for (size_t bucketIndex = 0; bucketIndex < self.buckets.count; bucketIndex++) {
        MELStringDictionaryBucket bucket = self.buckets.memory[bucketIndex];
        for (size_t entryIndex = 0; entryIndex < bucket.entries.count; entryIndex++) {
            MELStringDictionaryEntry entry = bucket.entries.memory[entryIndex];
            MELConstStringListPush(&self.keys, entry.key);
        }
    }
    return self;
}

void MELStringDictionaryEntryDeinit(MELStringDictionaryEntry * _Nonnull self) {
    free(self->key);
    free(self->value);
    self->key = NULL;
    self->value = NULL;
}

void MELStringDictionaryDeinit(MELStringDictionary * _Nonnull self) {
    for (size_t bucketIndex = 0; bucketIndex < self->buckets.capacity; bucketIndex++) {
        MELStringDictionaryBucket bucket = self->buckets.memory[bucketIndex];
        if (bucket.entries.memory != NULL) {
            MELStringDictionaryEntryListDeinitWithDeinitFunction(&self->buckets.memory[bucketIndex].entries, &MELStringDictionaryEntryDeinit);
        }
    }
    MELStringDictionaryBucketListDeinit(&self->buckets);
    MELConstStringListDeinit(&self->keys);
    self->count = 0;
}

void MELStringDictionaryGrowAndRehash(MELStringDictionary * _Nonnull self) {
    MELStringDictionaryBucketList oldBuckets = self->buckets;
    const size_t newCapacity = oldBuckets.capacity == 0 ? DEFAULT_BUCKET_COUNT : oldBuckets.capacity * 2;
    MELStringDictionaryBucketList newBuckets = MELStringDictionaryBucketListMakeWithInitialCapacity(newCapacity);
    if (newBuckets.memory == NULL) {
        fprintf(stderr, "Unable to grow dictionary of MELString to capacity %lun", newCapacity);
        return;
    }
    memset(newBuckets.memory, 0, sizeof(MELStringDictionaryBucketList) * newBuckets.capacity);
    newBuckets.count = oldBuckets.count;
    for (size_t bucketIndex = 0; bucketIndex < oldBuckets.capacity; bucketIndex++) {
        MELStringDictionaryBucket oldBucket = oldBuckets.memory[bucketIndex];
        for (size_t entryIndex = 0; entryIndex < oldBucket.entries.count; entryIndex++) {
            MELStringDictionaryEntry entry = oldBucket.entries.memory[entryIndex];

            size_t newBucketIndex = entry.hash % newBuckets.capacity;
            MELStringDictionaryEntryListPush(&newBuckets.memory[newBucketIndex].entries, entry);
        }
        MELStringDictionaryEntryListDeinit(&oldBucket.entries);
    }
    self->buckets = newBuckets;
    MELStringDictionaryBucketListDeinit(&oldBuckets);
}

char * _Nullable MELStringDictionaryPut(MELStringDictionary * _Nonnull self, const char * _Nonnull key, const char * _Nullable value) {
    if (MELStringDictionaryIsEmpty(*self)) {
        MELStringDictionaryGrowAndRehash(self);
    }
    const uint64_t hash = MELStringHash(key);
    const size_t bucketIndex = hash % self->buckets.capacity;
    MELStringDictionaryBucket bucket = self->buckets.memory[bucketIndex];
    if (bucket.entries.count > 0 && bucket.entries.memory != NULL) {
        for (size_t entryIndex = 0; entryIndex < bucket.entries.count; entryIndex++) {
            MELStringDictionaryEntry entry = bucket.entries.memory[entryIndex];
            if (entry.hash == hash && (entry.key == key || MELStringEquals(entry.key, key))) {
                MELString oldValue = entry.value;
                bucket.entries.memory[entryIndex].value = strdup(value);
                return oldValue;
            }
        }
    }
    if (bucket.entries.count == 0 || bucket.entries.memory == NULL) {
        self->buckets.count++;
    }
    self->count++;
    char * _Nonnull keyCopy = strdup(key);
    MELConstStringListPush(&self->keys, keyCopy);
    MELStringDictionaryEntryListPush(&self->buckets.memory[bucketIndex].entries, (MELStringDictionaryEntry) {hash, keyCopy, strdup(value)});

    const double loadFactor = self->count / (double) self->buckets.capacity;
    if (loadFactor >= LOAD_FACTOR) {
        MELStringDictionaryGrowAndRehash(self);
    }
    return NULL;
}

const char * _Nullable MELStringDictionaryGet(MELStringDictionary self, const char * _Nonnull key) {
    if (MELStringDictionaryIsEmpty(self)) {
        return NULL;
    }
    uint64_t hash = MELStringHash(key);
    size_t bucketIndex = hash % self.buckets.capacity;
    MELStringDictionaryBucket bucket = self.buckets.memory[bucketIndex];
    for (size_t entryIndex = 0; entryIndex < bucket.entries.count; entryIndex++) {
        MELStringDictionaryEntry entry = bucket.entries.memory[entryIndex];
        if (entry.hash == hash && (entry.key == key || MELStringEquals(entry.key, key))) {
            return entry.value;
        }
    }
    return NULL;
}

MELString MELStringDictionaryRemove(MELStringDictionary * _Nonnull self, const char * _Nonnull key) {
    if (MELStringDictionaryIsEmpty(*self)) {
        return NULL;
    }
    uint64_t hash = MELStringHash(key);
    size_t bucketIndex = hash % self->buckets.capacity;
    MELStringDictionaryBucket bucket = self->buckets.memory[bucketIndex];
    for (size_t entryIndex = 0; entryIndex < bucket.entries.count; entryIndex++) {
        MELStringDictionaryEntry entry = bucket.entries.memory[entryIndex];
        if (entry.hash == hash && (entry.key == key || MELStringEquals(entry.key, key))) {
            bucket.entries.memory[entryIndex].key = NULL;
            MELStringDictionaryEntryListRemove(&self->buckets.memory[bucketIndex].entries, entryIndex);
            int keyIndex = MELConstStringListIndexOf(self->keys, entry.key);
            MELConstStringListRemove(&self->keys, keyIndex);
            free(entry.key);
            self->count--;
            return entry.value;
        }
    }
    return NULL;
}

MELBoolean MELStringDictionaryContainsKey(MELStringDictionary self, const char * _Nonnull key) {
    if (MELStringDictionaryIsEmpty(self)) {
        return false;
    }
    uint64_t hash = MELStringHash(key);
    size_t bucketIndex = hash % self.buckets.capacity;
    MELStringDictionaryBucket bucket = self.buckets.memory[bucketIndex];
    for (size_t entryIndex = 0; entryIndex < bucket.entries.count; entryIndex++) {
        MELStringDictionaryEntry entry = bucket.entries.memory[entryIndex];
        if (entry.hash == hash && (entry.key == key || MELStringEquals(entry.key, key))) {
            return true;
        }
    }
    return false;
}

MELBoolean MELStringDictionaryIsEmpty(MELStringDictionary self) {
    return self.buckets.memory == 0 || self.buckets.memory == NULL;
}

MELStringDictionaryEntryList MELStringDictionaryEntries(MELStringDictionary * _Nonnull self) {
    MELStringDictionaryEntryList entries = MELStringDictionaryEntryListMake();

    MELStringDictionaryBucketList buckets = self->buckets;
    for (size_t bucketIndex = 0; bucketIndex < buckets.capacity; bucketIndex++) {
        MELStringDictionaryBucket bucket = buckets.memory[bucketIndex];
        for (size_t entryIndex = 0; entryIndex < bucket.entries.count; entryIndex++) {
            MELStringDictionaryEntry entry = bucket.entries.memory[entryIndex];
            MELStringDictionaryEntryListPush(&entries, entry);
        }
    }
    return entries;
}
