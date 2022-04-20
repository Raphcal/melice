//
//  stringdictionary.h
//  melice
//
//  Created by Raphaël Calabro on 20/04/2022.
//

#ifndef stringdictionary_h
#define stringdictionary_h

#include "melstd.h"

#include "list.h"
#include "melstring.h"

typedef struct {
    uint64_t hash;
    char * _Nonnull key;
    char * _Nullable value;
} MELStringDictionaryEntry;

MELListDefine(MELStringDictionaryEntry);

typedef struct {
    MELStringDictionaryEntryList entries;
} MELStringDictionaryBucket;

MELListDefine(MELStringDictionaryBucket);

/** Dictionary with string keys and MELString values. */
typedef struct {
    size_t count;
    MELStringDictionaryBucketList buckets;
    MELConstStringList keys;
} MELStringDictionary;

extern const MELStringDictionary MELStringDictionaryEmpty;
MELStringDictionary MELStringDictionaryMakeWithDictionary(MELStringDictionary other);
void MELStringDictionaryDeinit(MELStringDictionary * _Nonnull self);
char * _Nullable MELStringDictionaryPut(MELStringDictionary * _Nonnull self, const char * _Nonnull key, const char * _Nullable value);
const char * _Nullable MELStringDictionaryGet(MELStringDictionary self, const char * _Nonnull key);
char * _Nullable MELStringDictionaryRemove(MELStringDictionary * _Nonnull self, const char * _Nonnull key);
MELBoolean MELStringDictionaryContainsKey(MELStringDictionary self, const char * _Nonnull key);
MELBoolean MELStringDictionaryIsEmpty(MELStringDictionary self);
MELStringDictionaryEntryList MELStringDictionaryEntries(MELStringDictionary * _Nonnull self);

#endif /* stringdictionary_h */
