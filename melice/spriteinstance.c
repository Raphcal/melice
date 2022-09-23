//
//  spriteinstance.c
//  shmup
//
//  Created by Raphaël Calabro on 05/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "spriteinstance.h"

MELListImplement(MELSpriteInstance);

MELSpriteInstance MELSpriteInstanceMake(int32_t definitionIndex, MELPoint topLeft, int32_t zIndex, MELBoolean isUnique, char * _Nullable initializationScript) {
    return (MELSpriteInstance) {
        .definitionIndex = definitionIndex,
        .topLeft = topLeft,
        .zIndex = zIndex,
        .isUnique = isUnique,
        .initializationOperation = {NULL, 0},
        .initializationScript = initializationScript
    };
}

MELSpriteInstance MELSpriteInstanceMakeWithSpriteInstance(MELSpriteInstance other) {
    MELSpriteInstance self = other;
    self.initializationOperation = MELOperationMakeWithOperation(other.initializationOperation);
    self.initializationScript = MELStringCopy(other.initializationScript);
    return self;
}

MELSpriteInstance MELSpriteInstanceMakeWithInputStream(MELInputStream * _Nonnull inputStream) {
    MELSpriteInstance self;
    self.definitionIndex = MELInputStreamReadInt(inputStream);
    const int32_t x = MELInputStreamReadInt(inputStream);
    const int32_t y = MELInputStreamReadInt(inputStream);
    self.topLeft = MELPointMake((GLfloat)x, (GLfloat)y);
    self.isUnique = MELInputStreamReadBoolean(inputStream);
    
    if (MELInputStreamReadBoolean(inputStream)) {
        self.initializationOperation = MELOperationMakeWithInputStream(inputStream);
    } else {
        self.initializationOperation = (MELOperation) { NULL, 0 };
    }
    return self;
}

void MELSpriteInstanceDeinit(MELSpriteInstance * _Nonnull self) {
    MELOperationDeinit(&self->initializationOperation);
    free(self->initializationScript);
    self->initializationScript = NULL;
}
