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
    MELDecoratorTypeYFunction,
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

MELDecoratorRef MELDecoratorRefMakeWithDecoratorRef(MELDecoratorRef other);
void MELDecoratorRefDeinit(MELDecoratorRef * _Nonnull self);
MELDecoratorRef MELDecoratorRefListForType(MELDecoratorRefList self, MELDecoratorType type);

MELFunctionDecorator * _Nonnull MELFunctionDecoratorRefMakeWithFunctionDecoratorRef(MELFunctionDecorator * _Nonnull other);
void MELFunctionDecoratorDeinit(MELFunctionDecorator * _Nonnull self);
MELFunctionDecorator * _Nullable MELDecoratorRefListGetFunctionDecorator(MELDecoratorRefList self);
MELFunctionDecorator * _Nullable MELDecoratorRefListGetYFunctionDecorator(MELDecoratorRefList self);

MELHitboxDecorator * _Nonnull MELHitboxDecoratorRefMakeWithHitboxDecoratorRef(MELHitboxDecorator * _Nonnull other);
void MELHitboxDecoratorDeinit(MELHitboxDecorator * _Nonnull self);

MELSizeDecorator * _Nonnull MELSizeDecoratorRefMakeWithSizeDecoratorRef(MELSizeDecorator * _Nonnull other);
void MELSizeDecoratorDeinit(MELSizeDecorator * _Nonnull self);
MELSizeDecorator * _Nullable MELDecoratorRefListGetSizeDecorator(MELDecoratorRefList self);

#endif /* decorator_h */
