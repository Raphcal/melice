//
//  burstshootingstyle.h
//  Kuroobi
//
//  Created by Raphaël Calabro on 16/02/2023.
//

#ifndef burstshootingstyle_h
#define burstshootingstyle_h

#include "shootingstyle.h"
#include "shootingstyledefinition.h"

/**
 * Classe du style de tir burst.
 */
extern const MELShootingStyleClass MELBurstShootingStyleClass;

MELShootingStyle * _Nonnull MELBurstShootingStyleAlloc(const MELShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager);

#endif /* burstshootingstyle_h */
