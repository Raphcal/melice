//
//  aimedshootingstyle.h
//  shmup
//
//  Created by Raphaël Calabro on 15/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef aimedshootingstyle_h
#define aimedshootingstyle_h

#include "shootingstyle.h"
#include "shootingstyledefinition.h"

/**
 * Classe du style de tir guidé.
 */
extern const MELShootingStyleClass MELAimedShootingStyleClass;

typedef struct {
    MELShootingStyleDefinition super;
    MELSpriteType targetType;
} MELAimedShootingStyleDefinition;

MELAimedShootingStyleDefinition * _Nonnull MELAimedShootingStyleDefinitionAlloc(void);

MELShootingStyle * _Nonnull MELAimedShootingStyleAlloc(const MELAimedShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager);

MELShootingStyleDefinition * _Nonnull MELAimedShootingStyleCast(MELAimedShootingStyleDefinition * _Nonnull self);

#endif /* aimedshootingstyle_h */
