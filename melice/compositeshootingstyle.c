//
//  compositeshootingstyle.c
//  shmup
//
//  Created by Raphaël Calabro on 19/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "compositeshootingstyle.h"

#include <stdarg.h>

#pragma mark - CompositeShootingStyleDefinition

CompositeShootingStyleDefinition * _Nonnull CompositeShootingStyleDefinitionAlloc(unsigned int count, ...) {
    ShootingStyleDefinition **definitions = malloc((count + 1) * sizeof(ShootingStyleDefinition *));
    va_list ap;
    va_start(ap, count);
    for (unsigned int index = 0; index < count; index++) {
        definitions[index] = va_arg(ap, ShootingStyleDefinition *);
    }
    va_end(ap);
    definitions[count] = NULL;
    CompositeShootingStyleDefinition *self = malloc(sizeof(CompositeShootingStyleDefinition));
    *self = (CompositeShootingStyleDefinition) {
        (ShootingStyleDefinitionShootingStyleAlloc) &CompositeShootingStyleAlloc,
        definitions,
        count
    };
    return self;
}

void CompositeShootingStyleDefinitionDeinit(CompositeShootingStyleDefinition * _Nonnull self) {
    for (ShootingStyleDefinition **definition = self->definitions; definition != NULL; definition++) {
        free(*definition);
    }
    free(self->definitions);
    self->definitions = NULL;
}

#pragma mark - CompositeShootingStyle

ShootingStyle * _Nonnull CompositeShootingStyleAlloc(const CompositeShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    const unsigned int count = definition->count;
    ShootingStyle **styles = malloc((count + 1) * sizeof(ShootingStyle *));
    ShootingStyleDefinition **definitions = definition->definitions;
    
    for (int index = 0; index < count; index++) {
        ShootingStyleDefinition *definition = definitions[index];
        styles[index] = definition->shootingStyleAlloc(definition, spriteManager);
    }
    styles[count] = NULL;
    
    CompositeShootingStyle *self = malloc(sizeof(CompositeShootingStyle));
    *self = (CompositeShootingStyle) {
        &CompositeShootingStyleClass,
        definition,
        styles
    };
    return (ShootingStyle *)self;
}

void CompositeShootingStyleUpdate(CompositeShootingStyle * _Nonnull self, MELSprite * _Nonnull sprite, GLfloat angle, MELTimeInterval timeSinceLastUpdate) {
    for (ShootingStyle **styles = self->styles; *styles != NULL; styles++) {
        ShootingStyle *style = *styles;
        style->class->update(style, sprite, angle, timeSinceLastUpdate);
    }
}

const ShootingStyleClass CompositeShootingStyleClass = {
    .update = (void (*)(ShootingStyle *, MELSprite *, GLfloat, MELTimeInterval))&CompositeShootingStyleUpdate,
    .shoot = &NoShootingStyleShoot,
    .invert = &NoShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
