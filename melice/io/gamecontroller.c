//
//  gamecontroller.c
//  shmup
//
//  Created by Raphaël Calabro on 11/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "gamecontroller.h"

#include "inputsource.h"
#include "gamecontrollerlistener.h"

MELGameController MELGameControllerMakeWithButtonCount(size_t buttonCount) {
    MELGameController self;
    self.stick = MELPointZero;
    self.buttonStates = calloc(buttonCount, sizeof(MELButtonState));
    self.mappings = (MELList(MELButtonMappingRef)) MELListMake(MELButtonMappingRef);
    return self;
}
void MELGameControllerDeinit(MELGameController * _Nonnull self) {
    free(self->buttonStates);
    self->buttonStates = NULL;
    MELListDeinitWithDeinitFunction(self->mappings, MELButtonMappingRefDeinit);
}

void MELGameControllerRegisterMappings(MELGameController * _Nonnull self) {
    MELList(MELButtonMappingRef) mappings = self->mappings;
    for (int index = 0; index < mappings.count; index++) {
        MELButtonMapping *mapping = mappings.memory[index];
        MELInputSource *source = MELInputSourceForType(mapping->inputSourceType);
        if (source != NULL) {
            switch (mapping->mappingType) {
                case MELButtonMappingTypeStick:
                    source->class->addButtonListener(source, mapping, (MELGameControllerListener) { self, mapping->button, (void (* _Nonnull)(MELGameController * _Nonnull, int, MELBoolean)) &MELGameControllerDirectionStateChanged });
                    break;
                    
                case MELButtonMappingTypeButton:
                    source->class->addButtonListener(source, mapping, (MELGameControllerListener) { self, mapping->button, &MELGameControllerButtonStateChanged });
                    break;
                    
                default:
                    break;
            }
        }
    }
}

MELBoolean MELGameControllerIsPressingButton(MELGameController * _Nonnull self, int button) {
    return MELButtonStateIsPressing(self->buttonStates[button]);
}

MELBoolean MELGameControllerHasPressedButton(MELGameController * _Nonnull self, int button) {
    return MELButtonStateHasPressed(self->buttonStates + button);
}

void MELGameControllerButtonStateChanged(MELGameController * _Nonnull self, int button, MELBoolean pressed) {
    MELButtonState state = self->buttonStates[button];
    state.pressing = pressed;
    state.consumed = pressed && state.consumed;
    self->buttonStates[button] = state;
}

void MELGameControllerDirectionStateChanged(MELGameController * _Nonnull self, MELGameControllerDirection direction, MELBoolean pressed) {
    MELPoint stick = self->stick;
    switch (direction) {
        case MELGameControllerDirectionLeft:
            if (pressed) {
                stick.x = -1;
            } else if (stick.x < 0) {
                stick.x = 0;
            }
            break;
            
        case MELGameControllerDirectionRight:
            if (pressed) {
                stick.x = 1;
            } else if (stick.x > 0) {
                stick.x = 0;
            }
            break;
            
        case MELGameControllerDirectionUp:
            if (pressed) {
                stick.y = -1;
            } else if (stick.y < 0) {
                stick.y = 0;
            }
            break;
            
        case MELGameControllerDirectionDown:
            if (pressed) {
                stick.y = 1;
            } else if (stick.y > 0) {
                stick.y = 0;
            }
            break;
            
        default:
            break;
    }
    self->stick = stick;
}
