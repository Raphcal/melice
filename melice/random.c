//
//  random.c
//  shmup
//
//  Created by Raphaël Calabro on 15/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "random.h"

#include <time.h>

#ifdef __APPLE__

#define RANDOM_MAXIMUM 0x100000000
#define RANDOM_FUNCTION arc4random

void MELRandomInit(void) {
    // Pas d'initialisation.
}

#else

#define RANDOM_MAXIMUM RAND_MAX
#define RANDOM_FUNCTION rand

void MELRandomInit(void) {
    srand(time(NULL));
}

#endif

int MELRandomInt(int range) {
    return RANDOM_FUNCTION() % range;
}

GLfloat MELRandomFloat(GLfloat range) {
    return (GLfloat)((double)RANDOM_FUNCTION() / (double)RANDOM_MAXIMUM) * range;
}

MELTimeInterval MELRandomTimeInterval(MELTimeInterval range) {
    return (GLfloat)((double)RANDOM_FUNCTION() / (double)RANDOM_MAXIMUM) * range;
}

int MELRandomIntWithRange(int lower, int upper) {
    return MELRandomInt(upper - lower + 1) + lower;
}

GLfloat MELRandomFloatWithRange(GLfloat lower, GLfloat upper) {
    return MELRandomFloat(upper - lower) + lower;
}

MELTimeInterval MELRandomTimeIntervalWithRange(MELTimeInterval lower, MELTimeInterval upper) {
    return MELRandomTimeInterval(upper - lower) + lower;
}
