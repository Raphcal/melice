//
//  straightshootingstyle.h
//  shmup
//
//  Created by Raphaël Calabro on 16/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef straightshootingstyle_h
#define straightshootingstyle_h

#include "shootingstyle.h"
#include "shootingstyledefinition.h"

/**
 * Classe du style de tir droit.
 */
extern const MELShootingStyleClass MELStraightShootingStyleClass;

typedef struct {
    MELShootingStyleDefinition super;
    GLfloat space;
    MELPoint translation;
} MELStraightShootingStyleDefinition;

MELStraightShootingStyleDefinition * _Nonnull MELStraightShootingStyleDefinitionAlloc(void);

MELShootingStyle * _Nonnull MELStraightShootingStyleAlloc(const MELStraightShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager);

MELShootingStyleDefinition * _Nonnull MELStraightShootingStyleCast(MELStraightShootingStyleDefinition * _Nonnull self);

#endif /* straightshootingstyle_h */
