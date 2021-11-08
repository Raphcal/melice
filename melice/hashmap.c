//
//  hashmap.c
//  shmup
//
//  Created by Raphaël Calabro on 23/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "hashmap.h"

#define LOAD_FACTOR 0.75
#define DEFAULT_BUCKET_COUNT 16

MELListImplement(MELHashMapBucket);
MELListImplement(MELHashMapBucketEntry);

MELHashMapBucketEntry MELHashMapBucketEntryMake(uint64_t hash, void * _Nonnull key, void * _Nullable value) {
    return (MELHashMapBucketEntry) {hash, key, value};
}

MELHashMap MELHashMapMake(MELBoolean (* _Nonnull equals)(void * _Nonnull lhs, void * _Nonnull rhs), uint64_t (* _Nonnull hash)(void * _Nonnull key)) {
    return (MELHashMap) {{NULL, 0, 0}, 0, equals, hash};
}

void MELHashMapDeinit(MELHashMap * _Nonnull self) {
    for (size_t bucketIndex = 0; bucketIndex < self->buckets.capacity; bucketIndex++) {
        MELHashMapBucketEntryListDeinit(&self->buckets.memory[bucketIndex].entries);
    }
    MELHashMapBucketListDeinit(&self->buckets);
    self->count = 0;
}

void MELHashMapGrowAndRehash(MELHashMap * _Nonnull self) {
    MELHashMapBucketList oldBuckets = self->buckets;
    const size_t newCapacity = oldBuckets.capacity == 0 ? DEFAULT_BUCKET_COUNT : oldBuckets.capacity * 2;
    MELHashMapBucketList newBuckets = MELHashMapBucketListMakeWithInitialCapacity(newCapacity);
    if (newBuckets.memory == NULL) {
        fprintf(stderr, "Unable to grow hashmap to capacity %lu\n", newCapacity);
        return;
    }
    memset(newBuckets.memory, 0, sizeof(MELHashMapBucketList) * newBuckets.capacity);
    for (size_t bucketIndex = 0; bucketIndex < oldBuckets.capacity; bucketIndex++) {
        MELHashMapBucket bucket = oldBuckets.memory[bucketIndex];
        for (size_t entryIndex = 0; entryIndex < bucket.entries.count; entryIndex++) {
            MELHashMapBucketEntry entry = bucket.entries.memory[entryIndex];

            size_t newBucketIndex = entry.hash % newBuckets.capacity;
            MELHashMapBucketEntryListPush(&newBuckets.memory[newBucketIndex].entries, entry);
        }
    }
    self->buckets = newBuckets;
    MELHashMapBucketListDeinit(&oldBuckets);
}

void * _Nullable MELHashMapPut(MELHashMap * _Nonnull self, void * _Nonnull key, void * _Nullable value) {
    if (self->buckets.capacity == 0 || self->buckets.memory == NULL) {
        MELHashMapGrowAndRehash(self);
    }
    const uint64_t hash = self->hash(key);
    const size_t bucketIndex = hash % self->buckets.capacity;
    MELHashMapBucket bucket = self->buckets.memory[bucketIndex];
    if (bucket.entries.count > 0 && bucket.entries.memory != NULL) {
        for (size_t entryIndex = 0; entryIndex < bucket.entries.count; entryIndex++) {
            MELHashMapBucketEntry entry = bucket.entries.memory[entryIndex];
            if (entry.hash == hash && self->equals(entry.key, key)) {
                void * _Nullable oldValue = entry.value;
                entry.value = value;
                return oldValue;
            }
        }
    }
    if (bucket.entries.count == 0 || bucket.entries.memory == NULL) {
        self->buckets.count++;
    }
    self->count++;
    MELHashMapBucketEntryListPush(&self->buckets.memory[bucketIndex].entries, MELHashMapBucketEntryMake(hash, key, value));

    const double loadFactor = self->count / (double) self->buckets.capacity;
    if (loadFactor >= LOAD_FACTOR) {
        MELHashMapGrowAndRehash(self);
    }
    return NULL;
}

void * _Nullable MELHashMapGet(MELHashMap * _Nonnull self, void * _Nonnull key) {
    if (self->buckets.memory == 0 || self->buckets.memory == NULL) {
        return NULL;
    }
    uint64_t hash = self->hash(key);
    size_t bucketIndex = hash % self->buckets.capacity;
    MELHashMapBucket bucket = self->buckets.memory[bucketIndex];
    for (size_t entryIndex = 0; entryIndex < bucket.entries.count; entryIndex++) {
        MELHashMapBucketEntry entry = bucket.entries.memory[entryIndex];
        if (entry.hash == hash && self->equals(entry.key, key)) {
            return entry.value;
        }
    }
    return NULL;
}

MELBoolean stringEquals(char * _Nonnull lhs, char * _Nonnull rhs) {
    return strcmp(lhs, rhs) == 0;
}
uint64_t stringHash(char * _Nonnull key) {
    uint64_t hash = 0;
    while (*key) {
        hash ^= *key;
        key++;
    }
    return hash;
}
MELHashMap MELHashMapMakeWithStringKeys(void) {
    return MELHashMapMake((MELBoolean(*)(void *, void *))&stringEquals, (uint64_t(*)(void *))&stringHash);
}
