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

MELShootingStyle * _Nonnull MELAimedShootingStyleAlloc(const MELShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager);

#endif /* aimedshootingstyle_h */
