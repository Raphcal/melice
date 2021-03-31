//
//  inputsourcetype.h
//  shmup
//
//  Created by Raphaël Calabro on 11/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef inputsourcetype_h
#define inputsourcetype_h

#include "melstd.h"

typedef enum {
    
    /**
     *Inputs from the keyboard.
     */
    MELInputSourceTypeKeyboard,
    
    /**
     * Inputs from the mouse.
     */
    MELInputSourceTypeMouse,
    
    /**
     * Inputs from the touch screen.
     */
    MELInputSourceTypeTouchScreen,
    
    /**
     * Inputs from one of the connected gamepads.
     */
    MELInputSourceTypeGamepads

} MELInputSourceType;

#endif /* inputsourcetype_h */
