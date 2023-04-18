//
//  compositeshootingstyle.c
//  shmup
//
//  Created by Raphaël Calabro on 19/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "compositeshootingstyle.h"

#include <stdarg.h>

#pragma mark - MELCompositeShootingStyleDefinition

MELCompositeShootingStyleDefinition * _Nonnull MELCompositeShootingStyleDefinitionAlloc(unsigned int count, ...) {
    MELShootingStyleDefinition **definitions = malloc((count + 1) * sizeof(MELShootingStyleDefinition *));
    va_list ap;
    va_start(ap, count);
    for (unsigned int index = 0; index < count; index++) {
        definitions[index] = va_arg(ap, MELShootingStyleDefinition *);
    }
    va_end(ap);
    definitions[count] = NULL;
    MELCompositeShootingStyleDefinition *self = malloc(sizeof(MELCompositeShootingStyleDefinition));
    *self = (MELCompositeShootingStyleDefinition) {
        (MELShootingStyleDefinitionShootingStyleAlloc) &MELCompositeShootingStyleAlloc,
        definitions,
        count
    };
    return self;
}

void MELCompositeShootingStyleDefinitionDeinit(MELCompositeShootingStyleDefinition * _Nonnull self) {
    for (MELShootingStyleDefinition **definition = self->definitions; definition != NULL; definition++) {
        free(*definition);
    }
    free(self->definitions);
    self->definitions = NULL;
}

#pragma mark - MELCompositeShootingStyle

MELShootingStyle * _Nonnull MELCompositeShootingStyleAlloc(const MELCompositeShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    const unsigned int count = definition->count;
    MELShootingStyle **styles = malloc((count + 1) * sizeof(MELShootingStyle *));
    MELShootingStyleDefinition **definitions = definition->definitions;
    
    for (int index = 0; index < count; index++) {
        MELShootingStyleDefinition *definition = definitions[index];
        styles[index] = definition->shootingStyleAlloc(definition, spriteManager);
    }
    styles[count] = NULL;
    
    MELCompositeShootingStyle *self = malloc(sizeof(MELCompositeShootingStyle));
    *self = (MELCompositeShootingStyle) {
        &MELCompositeShootingStyleClass,
        definition,
        styles
    };
    return (MELShootingStyle *)self;
}

void MELCompositeShootingStyleUpdate(MELCompositeShootingStyle * _Nonnull self, MELSprite * _Nonnull sprite, GLfloat angle, MELTimeInterval timeSinceLastUpdate) {
    for (MELShootingStyle **styles = self->styles; *styles != NULL; styles++) {
        MELShootingStyle *style = *styles;
        style->class->update(style, sprite, angle, timeSinceLastUpdate);
    }
}

const MELShootingStyleClass MELCompositeShootingStyleClass = {
    .update = (void (*)(MELShootingStyle *, MELSprite *, GLfloat, MELTimeInterval))&MELCompositeShootingStyleUpdate,
    .shoot = &NoShootingStyleShoot,
    .invert = &NoShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
