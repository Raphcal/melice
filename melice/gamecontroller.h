//
//  gamecontroller.h
//  shmup
//
//  Created by Raphaël Calabro on 11/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef gamecontroller_h
#define gamecontroller_h

#include "melstd.h"

#include "buttonmapping.h"
#include "buttonstate.h"
#include "point.h"
#include "list.h"

typedef enum {
    MELGameControllerDirectionLeft,
    MELGameControllerDirectionRight,
    MELGameControllerDirectionUp,
    MELGameControllerDirectionDown
} MELGameControllerDirection;

typedef int MELGameControllerButton;

typedef struct melgamecontroller {
    MELPoint stick;
    MELButtonState * _Nullable buttonStates;
    MELList(MELButtonMappingRef) mappings;
} MELGameController;

MELGameController MELGameControllerMakeWithButtonCount(size_t buttonCount);
void MELGameControllerDeinit(MELGameController * _Nonnull self);

void MELGameControllerRegisterMappings(MELGameController * _Nonnull self);

MELBoolean MELGameControllerIsPressingButton(MELGameController * _Nonnull self, int button);

MELBoolean MELGameControllerHasPressedButton(MELGameController * _Nonnull self, int button);

void MELGameControllerButtonStateChanged(MELGameController * _Nonnull self, int button, MELBoolean pressed);

void MELGameControllerDirectionStateChanged(MELGameController * _Nonnull self, MELGameControllerDirection direction, MELBoolean pressed);

#endif /* gamecontroller_h */
