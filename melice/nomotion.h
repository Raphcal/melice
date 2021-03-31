//
//  noai.h
//  shmup
//
//  Created by Raphaël Calabro on 27/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef nomotion_h
#define nomotion_h

#include "melstd.h"

#include "motion.h"

/**
 * Class of the empty animation.
 */
extern const MELMotionClass MELNoMotionClass;

/**
 * Allocates and returns an empty ai.
 *
 * @return An empty ai.
 */
MELMotion * _Nonnull MELNoMotionAlloc(void);

#endif /* nomotion_h */
