//
//  inputsource.h
//  shmup
//
//  Created by Raphaël Calabro on 11/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef inputsource_h
#define inputsource_h

#include "melstd.h"

#include "inputsourcetype.h"
#include "gamecontroller.h"
#include "gamecontrollerlistener.h"
#include "list.h"

typedef struct melinputsource MELInputSource;

typedef MELInputSource * _Nonnull MELInputSourceRef;

typedef struct {
    MELInputSourceType type;
    void (* _Nonnull addButtonListener)(MELInputSource * _Nonnull self, MELButtonMapping * _Nonnull mapping, MELGameControllerListener listener);
    void (* _Nullable update)(MELInputSource * _Nonnull self, MELTimeInterval timeSinceLastUpdate);
} MELInputSourceClass;

#define MELInputSourceClassDefaults .update = NULL

struct melinputsource {
    const MELInputSourceClass * _Nonnull class;
};

MELListDefine(MELInputSourceRef);

/**
 * Register the given input source.
 * If an other input source exists with the same type, it will be overriden.
 *
 * @param source Input source to register.
 */
void MELInputSourceRegisterSource(MELInputSource * _Nonnull source);

/**
 * Returns the input source to use for the given type is any.
 *
 * @param type Type of source to return.
 * @return The input source or NULL if no input source is registered for the given type.
 */
MELInputSource * _Nullable MELInputSourceForType(MELInputSourceType type);

#endif /* inputsource_h */
