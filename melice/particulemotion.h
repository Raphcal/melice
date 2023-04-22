//
//  particulemotion.h
//  melice
//
//  Created by Raphaël Calabro on 22/04/2023.
//

#ifndef particulemotion_h
#define particulemotion_h

#include "melstd.h"

#include "motion.h"
#include "point.h"

extern const MELMotionClass MELParticuleMotionClass;

MELMotion * _Nonnull MELParticuleMotionAlloc(void);

#endif /* particulemotion_h */
