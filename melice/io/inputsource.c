//
//  inputsource.c
//  shmup
//
//  Created by Raphaël Calabro on 12/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "inputsource.h"

MELList(MELInputSourceRef) MELInputSources = MELListMakeEmpty(MELInputSourceRef);

void MELInputSourceRegisterSource(MELInputSource * _Nonnull source) {
    if (MELInputSources.count == 0) {
        MELListEnsureCapacity(MELInputSources, 4);
        for (int index = 0; index < 4; index++) {
            MELListPush(MELInputSources, NULL);
        }
    }
    MELInputSources.memory[source->class->type] = source;
}

MELInputSource * _Nullable MELInputSourceForType(MELInputSourceType type) {
    return type < MELInputSources.count ? MELInputSources.memory[type] : NULL;
}
