//
//  decorator.c
//  melice
//
//  Created by Raphaël Calabro on 15/11/2021.
//

#include "decorator.h"

MELListImplement(MELDecoratorRef);

void MELDecoratorDeinit(MELDecorator * _Nonnull self) {
    switch (self->type) {
        case MELDecoratorTypeFunction:
            MELFunctionDecoratorDeinit((MELFunctionDecorator *) self);
            break;
        case MELDecoratorTypeHitbox:
            MELHitboxDecoratorDeinit((MELHitboxDecorator *) self);
            break;
        case MELDecoratorTypeSize:
            MELSizeDecoratorDeinit((MELSizeDecorator *) self);
            break;
        default:
            fprintf(stderr, "Unsupported decorator type: %d\n", self->type);
            break;
    }
}

void MELDecoratorRefDeinit(MELDecoratorRef * _Nonnull self) {
    MELDecoratorDeinit(*self);
    free(*self);
    *self = NULL;
}

MELDecoratorRef MELDecoratorRefListForType(MELDecoratorRefList self, MELDecoratorType type) {
    for (unsigned int index = 0; index < self.count; index++) {
        if (self.memory[index]->type == type) {
            return self.memory[index];
        }
    }
    return NULL;
}

MELFunctionDecorator * _Nullable MELDecoratorRefListGetFunctionDecorator(MELDecoratorRefList self) {
    return (MELFunctionDecorator *)MELDecoratorRefListForType(self, MELDecoratorTypeFunction);
}

void MELFunctionDecoratorDeinit(MELFunctionDecorator * _Nonnull self) {
    free(self->function);
    self->function = NULL;
}

void MELHitboxDecoratorDeinit(MELHitboxDecorator * _Nonnull self) {
    self->hitbox = MELIntRectangleZero;
}

void MELSizeDecoratorDeinit(MELSizeDecorator * _Nonnull self) {
    self->size = MELIntSizeZero;
}
MELSizeDecorator * _Nullable MELDecoratorRefListGetSizeDecorator(MELDecoratorRefList self) {
    return (MELSizeDecorator *)MELDecoratorRefListForType(self, MELDecoratorTypeSize);
}
