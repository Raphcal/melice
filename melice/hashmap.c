//
//  hashmap.c
//  shmup
//
//  Created by Raphaël Calabro on 23/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "hashmap.h"

#include "melstring.h"

#define LOAD_FACTOR 0.75
#define DEFAULT_BUCKET_COUNT 16

MELListImplement(MELHashMapBucket);
MELListImplement(MELHashMapEntry);

MELHashMapEntry MELHashMapEntryMake(uint64_t hash, void * _Nonnull key, void * _Nullable value) {
    return (MELHashMapEntry) {hash, key, value};
}

MELHashMap MELHashMapMake(MELBoolean (* _Nonnull equals)(void * _Nonnull lhs, void * _Nonnull rhs), uint64_t (* _Nonnull hash)(void * _Nonnull key)) {
    return (MELHashMap) {{NULL, 0, 0}, 0, equals, hash};
}

MELHashMap MELHashMapMakeWithStringKeys(void) {
    // TODO: Should copy and free the keys !
    return MELHashMapMake((MELBoolean(*)(void *, void *))&MELStringEquals, (uint64_t(*)(void *))&MELStringHash);
}

void MELHashMapDeinit(MELHashMap * _Nonnull self) {
    for (size_t bucketIndex = 0; bucketIndex < self->buckets.capacity; bucketIndex++) {
        MELHashMapBucket bucket = self->buckets.memory[bucketIndex];
        if (bucket.entries.memory != NULL) {
            MELHashMapEntryListDeinit(&self->buckets.memory[bucketIndex].entries);
        }
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
    newBuckets.count = oldBuckets.count;
    for (size_t bucketIndex = 0; bucketIndex < oldBuckets.capacity; bucketIndex++) {
        MELHashMapBucket oldBucket = oldBuckets.memory[bucketIndex];
        for (size_t entryIndex = 0; entryIndex < oldBucket.entries.count; entryIndex++) {
            MELHashMapEntry entry = oldBucket.entries.memory[entryIndex];

            size_t newBucketIndex = entry.hash % newBuckets.capacity;
            MELHashMapEntryListPush(&newBuckets.memory[newBucketIndex].entries, entry);
        }
        MELHashMapEntryListDeinit(&oldBucket.entries);
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
            MELHashMapEntry entry = bucket.entries.memory[entryIndex];
            if (entry.hash == hash && (entry.key == key || self->equals(entry.key, key))) {
                void * _Nullable oldValue = entry.value;
                bucket.entries.memory[entryIndex].value = value;
                return oldValue;
            }
        }
    }
    if (bucket.entries.count == 0 || bucket.entries.memory == NULL) {
        self->buckets.count++;
    }
    self->count++;
    MELHashMapEntryListPush(&self->buckets.memory[bucketIndex].entries, MELHashMapEntryMake(hash, key, value));

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
        MELHashMapEntry entry = bucket.entries.memory[entryIndex];
        if (entry.hash == hash && (entry.key == key || self->equals(entry.key, key))) {
            return entry.value;
        }
    }
    return NULL;
}

void * _Nullable MELHashMapRemove(MELHashMap * _Nonnull self, void * _Nonnull key) {
    if (self->buckets.memory == 0 || self->buckets.memory == NULL) {
        return NULL;
    }
    uint64_t hash = self->hash(key);
    size_t bucketIndex = hash % self->buckets.capacity;
    MELHashMapBucket bucket = self->buckets.memory[bucketIndex];
    for (size_t entryIndex = 0; entryIndex < bucket.entries.count; entryIndex++) {
        MELHashMapEntry entry = bucket.entries.memory[entryIndex];
        if (entry.hash == hash && (entry.key == key || self->equals(entry.key, key))) {
            MELHashMapEntryListRemove(&self->buckets.memory[bucketIndex].entries, entryIndex);
            self->count--;
            return entry.value;
        }
    }
    return NULL;
}
