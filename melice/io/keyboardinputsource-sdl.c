//
//  keyboardinputsource-sdl.c
//  shmup
//
//  Created by Raphaël Calabro on 11/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "keyboardinputsource-sdl.h"

void MELSDLKeyboardInputSourceRegister(void) {
    MELSDLKeyboardInputSource *self = MELSDLKeyboardInputSourceGetInstance();
    MELInputSourceRegisterSource((MELInputSource *)self);
}

MELList(MELGameControllerListener) MELSDLKeyboardInputSourceGetListeners(MELSDLKeyboardInputSource * _Nonnull self, SDL_Scancode scancode) {
    MELList(MELListOfMELGameControllerListener) listeners = self->listeners;
    MELList(MELGameControllerListener) listener = scancode < listeners.count ? listeners.memory[scancode] : (MELList(MELGameControllerListener)) MELListMakeEmpty(MELGameControllerListener);
    return listener;
}

void MELSDLKeyboardInputSourceAddButtonListener(MELSDLKeyboardInputSource * _Nonnull self, MELSDLKeyboardButtonMapping * _Nonnull mapping, MELGameControllerListener listener) {
    SDL_Scancode scancode = mapping->scancode;
    MELList(MELGameControllerListener) scancodeListeners = MELSDLKeyboardInputSourceGetListeners(self, scancode);
    MELListPush(scancodeListeners, listener);
    MELListSet(self->listeners, scancode, scancodeListeners);
}

const MELInputSourceClass MELSDLKeyboardInputSourceClass = {
    MELInputSourceClassDefaults,
    .type = MELInputSourceTypeKeyboard,
    .addButtonListener = (void (*)(MELInputSource *, MELButtonMapping *, MELGameControllerListener))  &MELSDLKeyboardInputSourceAddButtonListener
};

MELSDLKeyboardInputSource * _Nonnull MELSDLKeyboardInputSourceGetInstance(void) {
    static MELSDLKeyboardInputSource * _Nullable MELSDLKeyboardInputSourceInstance = NULL;
    if (MELSDLKeyboardInputSourceInstance == NULL) {
        MELSDLKeyboardInputSourceInstance = malloc(sizeof(MELSDLKeyboardInputSource));
        *MELSDLKeyboardInputSourceInstance = (MELSDLKeyboardInputSource) {
            {
                &MELSDLKeyboardInputSourceClass
            },
            MELListMake(MELListOfMELGameControllerListener)
        };
    }
    return MELSDLKeyboardInputSourceInstance;
}

void MELSDLKeyboardInputSourceOnKeyDown(MELSDLKeyboardInputSource * _Nonnull self, SDL_Scancode scancode) {
    MELList(MELGameControllerListener) scancodeListeners = MELSDLKeyboardInputSourceGetListeners(self, scancode);
    for (int index = 0; index < scancodeListeners.count; index++) {
        MELGameControllerListenerCall(scancodeListeners.memory[index], true);
    }
}

void MELSDLKeyboardInputSourceOnKeyUp(MELSDLKeyboardInputSource * _Nonnull self, SDL_Scancode scancode) {
    MELList(MELGameControllerListener) scancodeListeners = MELSDLKeyboardInputSourceGetListeners(self, scancode);
    for (int index = 0; index < scancodeListeners.count; index++) {
        MELGameControllerListenerCall(scancodeListeners.memory[index], false);
    }
}

MELButtonMapping * _Nonnull MELSDLKeyboardButtonMappingAlloc(MELButtonMappingType type, int button, SDL_Scancode scancode) {
    MELSDLKeyboardButtonMapping *self = malloc(sizeof(MELSDLKeyboardButtonMapping));
    *self = (MELSDLKeyboardButtonMapping) {
        MELButtonMappingMake(MELInputSourceTypeKeyboard, type, button),
        scancode
    };
    return (MELButtonMapping *)self;
}

MELButtonMapping * _Nonnull MELSDLKeyboardButtonMappingAllocStick(MELGameControllerDirection direction, SDL_Scancode scancode) {
    return MELSDLKeyboardButtonMappingAlloc(MELButtonMappingTypeStick, direction, scancode);
}

MELButtonMapping * _Nonnull MELSDLKeyboardButtonMappingAllocButton(int button, SDL_Scancode scancode) {
    return MELSDLKeyboardButtonMappingAlloc(MELButtonMappingTypeButton, button, scancode);
}
