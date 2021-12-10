//
//  decorator.c
//  melice
//
//  Created by Raphaël Calabro on 15/11/2021.
//

#include "decorator.h"

MELListImplement(MELDecoratorRef);

#pragma mark - MELDecorator

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

#pragma mark - MELDecoratorRef

MELDecoratorRef MELDecoratorRefMakeWithDecoratorRef(MELDecoratorRef other) {
    if (other == NULL) {
        return NULL;
    }
    switch (other->type) {
        case MELDecoratorTypeFunction:
            return &MELFunctionDecoratorRefMakeWithFunctionDecoratorRef((MELFunctionDecorator *) other)->super;
        case MELDecoratorTypeHitbox:
            return &MELHitboxDecoratorRefMakeWithHitboxDecoratorRef((MELHitboxDecorator *) other)->super;
        case MELDecoratorTypeSize:
            return &MELSizeDecoratorRefMakeWithSizeDecoratorRef((MELSizeDecorator *) other)->super;
        default:
            fprintf(stderr, "Unsupported decorator type: %d\n", other->type);
            return NULL;
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

#pragma mark - MELFunctionDecorator

MELFunctionDecorator * _Nullable MELDecoratorRefListGetFunctionDecorator(MELDecoratorRefList self) {
    return (MELFunctionDecorator *)MELDecoratorRefListForType(self, MELDecoratorTypeFunction);
}

void MELFunctionDecoratorDeinit(MELFunctionDecorator * _Nonnull self) {
    free(self->function);
    self->function = NULL;
}

MELFunctionDecorator * _Nonnull MELFunctionDecoratorRefMakeWithFunctionDecoratorRef(MELFunctionDecorator * _Nonnull other) {
    MELFunctionDecorator *self = malloc(sizeof(MELFunctionDecorator));
    *self = (MELFunctionDecorator) {
        other->super,
        strdup(other->function)
    };
    return self;
}

#pragma mark - MELHitboxDecorator

void MELHitboxDecoratorDeinit(MELHitboxDecorator * _Nonnull self) {
    self->hitbox = MELIntRectangleZero;
}

MELHitboxDecorator * _Nonnull MELHitboxDecoratorRefMakeWithHitboxDecoratorRef(MELHitboxDecorator * _Nonnull other) {
    MELHitboxDecorator *self = malloc(sizeof(MELHitboxDecorator));
    *self = (MELHitboxDecorator) {
        other->super,
        other->hitbox
    };
    return self;
}

#pragma mark - MELSizeDecorator

void MELSizeDecoratorDeinit(MELSizeDecorator * _Nonnull self) {
    self->size = MELIntSizeZero;
}

MELSizeDecorator * _Nonnull MELSizeDecoratorRefMakeWithSizeDecoratorRef(MELSizeDecorator * _Nonnull other) {
    MELSizeDecorator *self = malloc(sizeof(MELSizeDecorator));
    *self = (MELSizeDecorator) {
        other->super,
        other->size
    };
    return self;
}

MELSizeDecorator * _Nullable MELDecoratorRefListGetSizeDecorator(MELDecoratorRefList self) {
    return (MELSizeDecorator *)MELDecoratorRefListForType(self, MELDecoratorTypeSize);
}
