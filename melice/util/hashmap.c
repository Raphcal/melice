//
//  hashmap.c
//  shmup
//
//  Created by Raphaël Calabro on 23/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "hashmap.h"

struct bucket {
    long hash;
    void *key;
    void *value;
};

struct melhashmap {
    size_t count;
    struct bucket * _Nonnull buckets;
    long (*hash)(void * _Nonnull key);
};
