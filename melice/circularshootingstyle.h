//
//  circularshootingstyle.h
//  shmup
//
//  Created by Raphaël Calabro on 18/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef circularshootingstyle_h
#define circularshootingstyle_h

#include "shootingstyle.h"
#include "shootingstyledefinition.h"

/**
 * Classe du style de tir en cercles.
 */
extern const MELShootingStyleClass MELCircularShootingStyleClass;

typedef struct {
    MELShootingStyleDefinition super;

    /// Angle du premier tir.
    GLfloat baseAngle;
    
    /// Variation de l'angle de départ.
    GLfloat baseAngleVariation;
    
    /// Différence d'angle entre 2 tirs.
    GLfloat angleIncrement;
} MELCircularShootingStyleDefinition;

typedef struct {
    MELShootingStyle super;
    GLfloat baseAngle;
    GLfloat baseAngleVariation;
} MELCircularShootingStyle;

MELCircularShootingStyleDefinition * _Nonnull MELCircularShootingStyleDefinitionAlloc(void);

MELShootingStyle * _Nonnull MELCircularShootingStyleAlloc(const MELCircularShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager);

MELShootingStyleDefinition * _Nonnull MELCircularShootingStyleCast(MELCircularShootingStyleDefinition * _Nonnull self);

#endif /* circularshootingstyle_h */
