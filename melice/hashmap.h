//
//  hashmap.h
//  shmup
//
//  Created by Raphaël Calabro on 23/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef hashmap_h
#define hashmap_h

#include "melstd.h"

#include "list.h"

typedef struct {
    uint64_t hash;
    void * _Nonnull key;
    void * _Nullable value;
} MELHashMapBucketEntry;

MELListDefine(MELHashMapBucketEntry);

typedef struct {
    MELList(MELHashMapBucketEntry) entries;
} MELHashMapBucket;

MELListDefine(MELHashMapBucket);

typedef struct melhashmap {
    MELList(MELHashMapBucket) buckets;
    size_t count;
    MELBoolean (* _Nonnull equals)(void * _Nonnull lhs, void * _Nonnull rhs);
    uint64_t (* _Nonnull hash)(void * _Nonnull key);
} MELHashMap;

MELHashMap MELHashMapMake(MELBoolean (* _Nonnull equals)(void * _Nonnull lhs, void * _Nonnull rhs), uint64_t (* _Nonnull hash)(void * _Nonnull key));
MELHashMap MELHashMapMakeWithStringKeys(void);

void MELHashMapDeinit(MELHashMap * _Nonnull self);

void * _Nullable MELHashMapPut(MELHashMap * _Nonnull self, void * _Nonnull key, void * _Nullable value);

void * _Nullable MELHashMapGet(MELHashMap * _Nonnull self, void * _Nonnull key);

#endif /* hashmap_h */
