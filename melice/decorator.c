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
        default:
            break;
    }
}

void MELDecoratorRefDeinit(MELDecoratorRef * _Nonnull self) {
    MELDecoratorDeinit(*self);
    free(*self);
    *self = NULL;
}

void MELFunctionDecoratorDeinit(MELFunctionDecorator * _Nonnull self) {
    free(self->function);
    self->function = NULL;
}

void MELHitboxDecoratorDeinit(MELHitboxDecorator * _Nonnull self) {
    self->hitbox = MELIntRectangleZero;
}
