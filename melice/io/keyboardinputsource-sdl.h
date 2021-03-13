//
//  keyboardinputsource-sdl.h
//  shmup
//
//  Created by Raphaël Calabro on 11/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef keyboardinputsource_sdl_h
#define keyboardinputsource_sdl_h

#include "../melstd.h"

#include <SDL2/SDL.h>

#include "buttonmapping.h"
#include "inputsource.h"

extern const MELInputSourceClass MELSDLKeyboardInputSourceClass;

typedef struct {
    MELInputSource super;
    MELList(MELListOfMELGameControllerListener) listeners;
} MELSDLKeyboardInputSource;

typedef struct {
    MELButtonMapping super;
    SDL_Scancode scancode;
} MELSDLKeyboardButtonMapping;

void MELSDLKeyboardInputSourceRegister(void);

MELSDLKeyboardInputSource * _Nonnull MELSDLKeyboardInputSourceGetInstance(void);

void MELSDLKeyboardInputSourceOnKeyDown(MELSDLKeyboardInputSource * _Nonnull self, SDL_Scancode scancode);

void MELSDLKeyboardInputSourceOnKeyUp(MELSDLKeyboardInputSource * _Nonnull self, SDL_Scancode scancode);

MELButtonMapping * _Nonnull MELSDLKeyboardButtonMappingAlloc(MELButtonMappingType type, int button, SDL_Scancode scancode);

MELButtonMapping * _Nonnull MELSDLKeyboardButtonMappingAllocStick(MELGameControllerDirection direction, SDL_Scancode scancode);

MELButtonMapping * _Nonnull MELSDLKeyboardButtonMappingAllocButton(int button, SDL_Scancode scancode);

#endif /* keyboardinputsource_sdl_h */
