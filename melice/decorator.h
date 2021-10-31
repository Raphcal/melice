//
//  decorator.h
//  melice
//
//  Created by Raphaël Calabro on 31/10/2021.
//

#ifndef decorator_h
#define decorator_h

#include "melstd.h"

typedef enum {
    MELDecoratorTypeHitbox,
    MELDecoratorTypeFunction,
    MELDecoratorTypeFlag,
} MELDecoratorType;

typedef struct {
    MELDecoratorType type;
} MELDecorator;

#endif /* decorator_h */
