//
//  animationtype.c
//  shmup
//
//  Created by Raphaël Calabro on 04/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "animationtype.h"

MELAnimationType MELAnimationTypeForFrameCountAndLooping(int frameCount, MELBoolean looping) {
    if (frameCount == 1) {
        return MELAnimationTypeSingleFrame;
    } else if (looping) {
        return MELAnimationTypeLooping;
    } else if (frameCount > 1) {
        return MELAnimationTypePlayOnce;
    } else {
        return MELAnimationTypeNone;
    }
}
