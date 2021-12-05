//
//  decorator.h
//  melice
//
//  Created by Raphaël Calabro on 31/10/2021.
//

#ifndef decorator_h
#define decorator_h

#include "melstd.h"

#include "list.h"
#include "rectangle.h"

typedef enum {
    MELDecoratorTypeHitbox,
    MELDecoratorTypeFunction,
    MELDecoratorTypeFlag,
} MELDecoratorType;

typedef struct {
    MELDecoratorType type;
} MELDecorator;

typedef MELDecorator * _Nullable MELDecoratorRef;

MELListDefine(MELDecoratorRef);

void MELDecoratorDeinit(MELDecorator * _Nonnull self);

void MELDecoratorRefDeinit(MELDecoratorRef * _Nonnull self);

typedef struct {
    MELDecorator super;
    char * _Nonnull function;
} MELFunctionDecorator;

void MELFunctionDecoratorDeinit(MELFunctionDecorator * _Nonnull self);

typedef struct {
    MELDecorator super;
    MELIntRectangle hitbox;
} MELHitboxDecorator;

void MELHitboxDecoratorDeinit(MELHitboxDecorator * _Nonnull self);

#endif /* decorator_h */
