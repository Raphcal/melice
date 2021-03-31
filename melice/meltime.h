//
//  meltime.h
//  shmup
//
//  Created by Raphaël Calabro on 02/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef time_h
#define time_h

#include "melstd.h"

#ifndef _WIN32
#include <sys/time.h>
#else
#include <time.h>
int nanosleep(const struct timespec *rqtp, struct timespec *rmtp);
#endif

/**
 * Returns the time in nanoseconds.
 *
 * @return The time in nanoseconds.
 */
int64_t MELNanoTime(void);

#endif /* time_h */
