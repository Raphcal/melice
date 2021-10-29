//
//  buttonmapping.c
//  shmup
//
//  Created by Raphaël Calabro on 13/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "buttonmapping.h"

MELListImplement(MELButtonMappingRef);

MELButtonMapping MELButtonMappingMake(MELInputSourceType inputSourceType, MELButtonMappingType mappingType, int button) {
    return (MELButtonMapping) {
        inputSourceType,
        mappingType,
        button
    };
}

void MELButtonMappingRefDeinit(MELButtonMappingRef * _Nonnull mappingRef) {
    free(*mappingRef);
    *mappingRef = NULL;
}
