//
//  referencepool.h
//  shmup
//
//  Created by Raphaël Calabro on 22/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef referencepool_h
#define referencepool_h

#include "melstd.h"

typedef struct {
    unsigned int * _Nullable available;
    unsigned int count;
    unsigned int capacity;
} MELReferencePool;

MELReferencePool MELReferencePoolMakeWithRange(unsigned int from, unsigned int to);

void MELReferencePoolDeinit(MELReferencePool * _Nonnull self);

unsigned int MELReferencePoolNextReference(MELReferencePool * _Nonnull self);

void MELReferencePoolReleaseReference(MELReferencePool * _Nonnull self, unsigned int reference);

#endif /* referencepool_h */
