//
//  buttonstate.h
//  shmup
//
//  Created by Raphaël Calabro on 11/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef buttonstate_h
#define buttonstate_h

#include "melstd.h"

typedef struct {
    MELBoolean pressing;
    MELBoolean consumed;
} MELButtonState;

MELBoolean MELButtonStateIsPressing(MELButtonState self);

MELBoolean MELButtonStateHasPressed(MELButtonState * _Nonnull self);

#endif /* buttonstate_h */
