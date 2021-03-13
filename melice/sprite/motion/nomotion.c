//
//  noai.c
//  shmup
//
//  Created by Raphaël Calabro on 27/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "nomotion.h"

const MELMotionClass MELNoMotionClass = {
    MELMotionClassDefaults
};

MELMotion * _Nonnull MELNoMotionAlloc(void) {
    MELMotion *self = malloc(sizeof(MELMotion));
    *self = (MELMotion) {
        &MELNoMotionClass
    };
    return self;
}
