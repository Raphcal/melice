//
//  gamecontrollerlistener.c
//  shmup
//
//  Created by Raphaël Calabro on 13/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "gamecontrollerlistener.h"

MELListImplement(MELGameControllerListener);
MELListImplement(MELGameControllerListenerList);

void MELGameControllerListenerCall(MELGameControllerListener self, MELBoolean pressed) {
    self.listener(self.controller, self.button, pressed);
}
