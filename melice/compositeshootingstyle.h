//
//  compositeshootingstyle.h
//  shmup
//
//  Created by Raphaël Calabro on 19/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef compositeshootingstyle_h
#define compositeshootingstyle_h

#include "shootingstyle.h"
#include "shootingstyledefinition.h"

#pragma mark - Definition

typedef struct {
    ShootingStyleDefinitionShootingStyleAlloc shootingStyleAlloc;
    ShootingStyleDefinition * _Nonnull * _Nullable definitions;
    unsigned int count;
} CompositeShootingStyleDefinition;

CompositeShootingStyleDefinition * _Nonnull CompositeShootingStyleDefinitionAlloc(unsigned int count, ...);

void CompositeShootingStyleDefinitionDeinit(CompositeShootingStyleDefinition * _Nonnull self);

#pragma mark - Shooting style

extern const ShootingStyleClass CompositeShootingStyleClass;

typedef struct {
    const ShootingStyleClass * _Nonnull class;
    const CompositeShootingStyleDefinition * _Nonnull definition;
    ShootingStyle * _Nonnull * _Nullable styles;
} CompositeShootingStyle;

ShootingStyle * _Nonnull CompositeShootingStyleAlloc(const CompositeShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager);

#endif /* compositeshootingstyle_h */
