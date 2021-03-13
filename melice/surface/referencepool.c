//
//  referencepool.c
//  shmup
//
//  Created by Raphaël Calabro on 22/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "referencepool.h"

#include <assert.h>

MELReferencePool MELReferencePoolMakeWithRange(unsigned int from, unsigned int to) {
    assert(to >= from);
    
    const unsigned int count = to - from;
    
    MELReferencePool self;
    self.available = calloc(count, sizeof(unsigned int));
    self.count = count;
    self.capacity = count;
    
    const unsigned int reference = to - 1;
    for (unsigned int index = 0; index < count; index++) {
        self.available[index] = reference - count;
    }
    
    return self;
}

void MELReferencePoolDeinit(MELReferencePool * _Nonnull self) {
    free(self->available);
    self->available = NULL;
    self->count = 0;
    self->capacity = 0;
}

unsigned int MELReferencePoolNextReference(MELReferencePool * _Nonnull self) {
    assert(self->count > 0);
    return self->available[--self->count];
}

void MELReferencePoolReleaseReference(MELReferencePool * _Nonnull self, unsigned int reference) {
    assert(self->count < self->capacity);
    self->available[self->count++] = reference;
}
