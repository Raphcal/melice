//
//  gamecontrollerlistener.h
//  shmup
//
//  Created by Raphaël Calabro on 13/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef gamecontrollerlistener_h
#define gamecontrollerlistener_h

#include "../melstd.h"

#include "../util/list.h"

typedef struct melgamecontroller MELGameController;

typedef struct {
    MELGameController * _Nonnull controller;
    int button;
    void (* _Nonnull listener)(MELGameController * _Nonnull, int, MELBoolean);
} MELGameControllerListener;

MELListDefine(MELGameControllerListener);
MELListDefine(MELListOfMELGameControllerListener);

void MELGameControllerListenerCall(MELGameControllerListener self, MELBoolean pressed);

#endif /* gamecontrollerlistener_h */
