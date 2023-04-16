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
extern const ShootingStyleClass CircularShootingStyleClass;

typedef struct {
    ShootingStyleDefinition super;

    /// Angle du premier tir.
    GLfloat baseAngle;
    
    /// Variation de l'angle de départ.
    GLfloat baseAngleVariation;
    
    /// Différence d'angle entre 2 tirs.
    GLfloat angleIncrement;
} CircularShootingStyleDefinition;

typedef struct {
    ShootingStyle super;
    GLfloat baseAngle;
    GLfloat baseAngleVariation;
} CircularShootingStyle;

CircularShootingStyleDefinition * _Nonnull CircularShootingStyleDefinitionAlloc(void);

ShootingStyle * _Nonnull CircularShootingStyleAlloc(const CircularShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager);

#endif /* circularshootingstyle_h */
