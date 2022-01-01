//
//  dictionary.h
//  melice
//
//  Created by Raphaël Calabro on 09/11/2021.
//

#ifndef dictionary_h
#define dictionary_h

#include "melstd.h"

#include <string.h>
#include "list.h"
#include "melstring.h"

#define LOAD_FACTOR 0.75
#define DEFAULT_BUCKET_COUNT 16

// Definition macro

#define MELDictionaryDefine(type) typedef struct {\
uint64_t hash;\
char * _Nonnull key;\
type value;\
} type##DictionaryEntry;\
\
MELListDefine(type##DictionaryEntry);\
\
typedef struct {\
MELList(type##DictionaryEntry) entries;\
} type##DictionaryBucket;\
\
MELListDefine(type##DictionaryBucket);\
\
/** Dictionary with string keys and type values. */ typedef struct {\
MELList(type##DictionaryBucket) buckets;\
size_t count;\
} type##Dictionary;\
\
extern const type##Dictionary type##DictionaryEmpty;\
void type##DictionaryDeinit(type##Dictionary * _Nonnull self);\
type type##DictionaryPut(type##Dictionary * _Nonnull self, const char * _Nonnull key, type value);\
type type##DictionaryGet(type##Dictionary self, const char * _Nonnull key);\
MELBoolean type##DictionaryGetIfPresent(type##Dictionary self, const char * _Nonnull key, type * _Nonnull value);\
type type##DictionaryRemove(type##Dictionary * _Nonnull self, const char * _Nonnull key);\
type##DictionaryEntryList type##DictionaryEntries(type##Dictionary * _Nonnull self);

// Implementation macro

#define MELDictionaryImplement(type, nil) const type##Dictionary type##DictionaryEmpty = {{NULL, 0, 0}, 0};\
\
MELListImplement(type##DictionaryBucket);\
MELListImplement(type##DictionaryEntry);\
\
void type##DictionaryEntryDeinit(type##DictionaryEntry * _Nonnull self) {\
    free(self->key);\
    self->key = NULL;\
}\
\
void type##DictionaryDeinit(type##Dictionary * _Nonnull self) {\
    for (size_t bucketIndex = 0; bucketIndex < self->buckets.capacity; bucketIndex++) {\
        type##DictionaryBucket bucket = self->buckets.memory[bucketIndex];\
        if (bucket.entries.memory != NULL) {\
            type##DictionaryEntryListDeinitWithDeinitFunction(&self->buckets.memory[bucketIndex].entries, &type##DictionaryEntryDeinit);\
        }\
    }\
    type##DictionaryBucketListDeinit(&self->buckets);\
    self->count = 0;\
}\
\
void type##DictionaryGrowAndRehash(type##Dictionary * _Nonnull self) {\
    type##DictionaryBucketList oldBuckets = self->buckets;\
    const size_t newCapacity = oldBuckets.capacity == 0 ? DEFAULT_BUCKET_COUNT : oldBuckets.capacity * 2;\
    type##DictionaryBucketList newBuckets = type##DictionaryBucketListMakeWithInitialCapacity(newCapacity);\
    if (newBuckets.memory == NULL) {\
        fprintf(stderr, "Unable to grow dictionary of type to capacity %lu\n", newCapacity);\
        return;\
    }\
    memset(newBuckets.memory, 0, sizeof(type##DictionaryBucketList) * newBuckets.capacity);\
    newBuckets.count = oldBuckets.count;\
    for (size_t bucketIndex = 0; bucketIndex < oldBuckets.capacity; bucketIndex++) {\
        type##DictionaryBucket oldBucket = oldBuckets.memory[bucketIndex];\
        for (size_t entryIndex = 0; entryIndex < oldBucket.entries.count; entryIndex++) {\
            type##DictionaryEntry entry = oldBucket.entries.memory[entryIndex];\
\
            size_t newBucketIndex = entry.hash % newBuckets.capacity;\
            type##DictionaryEntryListPush(&newBuckets.memory[newBucketIndex].entries, entry);\
        }\
        type##DictionaryEntryListDeinit(&oldBucket.entries);\
    }\
    self->buckets = newBuckets;\
    type##DictionaryBucketListDeinit(&oldBuckets);\
}\
\
type type##DictionaryPut(type##Dictionary * _Nonnull self, const char * _Nonnull key, type value) {\
    if (self->buckets.capacity == 0 || self->buckets.memory == NULL) {\
        type##DictionaryGrowAndRehash(self);\
    }\
    const uint64_t hash = MELStringHash(key);\
    const size_t bucketIndex = hash % self->buckets.capacity;\
    type##DictionaryBucket bucket = self->buckets.memory[bucketIndex];\
    if (bucket.entries.count > 0 && bucket.entries.memory != NULL) {\
        for (size_t entryIndex = 0; entryIndex < bucket.entries.count; entryIndex++) {\
            type##DictionaryEntry entry = bucket.entries.memory[entryIndex];\
            if (entry.hash == hash && (entry.key == key || MELStringEquals(entry.key, key))) {\
                type oldValue = entry.value;\
                entry.value = value;\
                return oldValue;\
            }\
        }\
    }\
    if (bucket.entries.count == 0 || bucket.entries.memory == NULL) {\
        self->buckets.count++;\
    }\
    self->count++;\
    char *keyCopy = strdup(key);\
    type##DictionaryEntryListPush(&self->buckets.memory[bucketIndex].entries, (type##DictionaryEntry) {hash, keyCopy, value});\
\
    const double loadFactor = self->count / (double) self->buckets.capacity;\
    if (loadFactor >= LOAD_FACTOR) {\
        type##DictionaryGrowAndRehash(self);\
    }\
    return nil;\
}\
\
type type##DictionaryGet(type##Dictionary self, const char * _Nonnull key) {\
    type value;\
    return type##DictionaryGetIfPresent(self, key, &value)\
        ? value\
        : nil;\
}\
\
MELBoolean type##DictionaryGetIfPresent(type##Dictionary self, const char * _Nonnull key, type * _Nonnull value) {\
    if (self.buckets.memory == 0 || self.buckets.memory == NULL) {\
        return false;\
    }\
    uint64_t hash = MELStringHash(key);\
    size_t bucketIndex = hash % self.buckets.capacity;\
    type##DictionaryBucket bucket = self.buckets.memory[bucketIndex];\
    for (size_t entryIndex = 0; entryIndex < bucket.entries.count; entryIndex++) {\
        type##DictionaryEntry entry = bucket.entries.memory[entryIndex];\
        if (entry.hash == hash && (entry.key == key || MELStringEquals(entry.key, key))) {\
            *value = entry.value;\
            return true;\
        }\
    }\
    return false;\
}\
\
type type##DictionaryRemove(type##Dictionary * _Nonnull self, const char * _Nonnull key) {\
    if (self->buckets.memory == 0 || self->buckets.memory == NULL) {\
        return nil;\
    }\
    uint64_t hash = MELStringHash(key);\
    size_t bucketIndex = hash % self->buckets.capacity;\
    type##DictionaryBucket bucket = self->buckets.memory[bucketIndex];\
    for (size_t entryIndex = 0; entryIndex < bucket.entries.count; entryIndex++) {\
        type##DictionaryEntry entry = bucket.entries.memory[entryIndex];\
        if (entry.hash == hash && (entry.key == key || MELStringEquals(entry.key, key))) {\
            bucket.entries.memory[entryIndex].key = NULL;\
            type##DictionaryEntryListRemove(&self->buckets.memory[bucketIndex].entries, entryIndex);\
            free(entry.key);\
            self->count--;\
            return entry.value;\
        }\
    }\
    return nil;\
}\
\
type##DictionaryEntryList type##DictionaryEntries(type##Dictionary * _Nonnull self) {\
    type##DictionaryEntryList entries = type##DictionaryEntryListMake();\
\
    type##DictionaryBucketList buckets = self->buckets;\
    for (size_t bucketIndex = 0; bucketIndex < buckets.capacity; bucketIndex++) {\
        type##DictionaryBucket bucket = buckets.memory[bucketIndex];\
        for (size_t entryIndex = 0; entryIndex < bucket.entries.count; entryIndex++) {\
            type##DictionaryEntry entry = bucket.entries.memory[entryIndex];\
            type##DictionaryEntryListPush(&entries, entry);\
        }\
    }\
    return entries;\
}

#endif /* dictionary_h */
