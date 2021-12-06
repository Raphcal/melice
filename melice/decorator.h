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
#include "size.h"

typedef enum {
    MELDecoratorTypeHitbox,
    MELDecoratorTypeFunction,
    MELDecoratorTypeFlag,
    MELDecoratorTypeSize,
} MELDecoratorType;

typedef struct {
    MELDecoratorType type;
} MELDecorator;

typedef struct {
    MELDecorator super;
    MELIntRectangle hitbox;
} MELHitboxDecorator;

typedef struct {
    MELDecorator super;
    char * _Nonnull function;
} MELFunctionDecorator;

typedef struct {
    MELDecorator super;
    MELIntSize size;
} MELSizeDecorator;

typedef MELDecorator * _Nullable MELDecoratorRef;

MELListDefine(MELDecoratorRef);

void MELDecoratorDeinit(MELDecorator * _Nonnull self);
void MELDecoratorRefDeinit(MELDecoratorRef * _Nonnull self);
MELDecoratorRef MELDecoratorRefListForType(MELDecoratorRefList self, MELDecoratorType type);

void MELFunctionDecoratorDeinit(MELFunctionDecorator * _Nonnull self);
MELFunctionDecorator * _Nullable MELDecoratorRefListGetFunctionDecorator(MELDecoratorRefList self);

void MELHitboxDecoratorDeinit(MELHitboxDecorator * _Nonnull self);

void MELSizeDecoratorDeinit(MELSizeDecorator * _Nonnull self);
MELSizeDecorator * _Nullable MELDecoratorRefListGetSizeDecorator(MELDecoratorRefList self);

#endif /* decorator_h */
