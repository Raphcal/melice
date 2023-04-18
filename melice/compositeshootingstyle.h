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
    MELShootingStyleDefinitionShootingStyleAlloc shootingStyleAlloc;
    MELShootingStyleDefinition * _Nonnull * _Nullable definitions;
    unsigned int count;
} MELCompositeShootingStyleDefinition;

MELCompositeShootingStyleDefinition * _Nonnull MELCompositeShootingStyleDefinitionAlloc(unsigned int count, ...);

void MELCompositeShootingStyleDefinitionDeinit(MELCompositeShootingStyleDefinition * _Nonnull self);

#pragma mark - Shooting style

extern const MELShootingStyleClass MELCompositeShootingStyleClass;

typedef struct {
    const MELShootingStyleClass * _Nonnull class;
    const MELCompositeShootingStyleDefinition * _Nonnull definition;
    MELShootingStyle * _Nonnull * _Nullable styles;
} MELCompositeShootingStyle;

MELShootingStyle * _Nonnull MELCompositeShootingStyleAlloc(const MELCompositeShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager);

#endif /* compositeshootingstyle_h */
