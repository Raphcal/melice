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
    MELShootingStyle super;
    GLfloat baseAngle;
    GLfloat baseAngleVariation;
} MELCircularShootingStyle;

MELShootingStyle * _Nonnull MELCircularShootingStyleAlloc(const MELShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager);

#endif /* circularshootingstyle_h */
