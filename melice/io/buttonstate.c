//
//  buttonstate.c
//  shmup
//
//  Created by Raphaël Calabro on 12/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "buttonstate.h"

MELBoolean MELButtonStateIsPressing(MELButtonState self) {
    return self.pressing;
}

MELBoolean MELButtonStateHasPressed(MELButtonState * _Nonnull self) {
    const MELBoolean pressed = self->pressing && !self->consumed;
    self->consumed = self->pressing;
    return pressed;
}
