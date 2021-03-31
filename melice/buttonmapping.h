//
//  gamecontrollerbutton.h
//  shmup
//
//  Created by Raphaël Calabro on 11/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef buttonmapping_h
#define buttonmapping_h

#include "melstd.h"

#include "inputsourcetype.h"
#include "list.h"

typedef enum {
    MELButtonMappingTypeStick,
    MELButtonMappingTypeButton
} MELButtonMappingType;

typedef struct {
    MELInputSourceType inputSourceType;
    MELButtonMappingType mappingType;
    int button;
} MELButtonMapping;

typedef MELButtonMapping * _Nonnull MELButtonMappingRef;

MELListDefine(MELButtonMappingRef);

MELButtonMapping MELButtonMappingMake(MELInputSourceType inputSourceType, MELButtonMappingType mappingType, int button);

void MELButtonMappingRefDeinit(MELButtonMappingRef * _Nonnull mappingRef);

#endif /* buttonmapping_h */
