//
//  particuleshootingstyle.h
//  melice
//
//  Created by Raphaël Calabro on 22/04/2023.
//

#ifndef particuleshootingstyle_h
#define particuleshootingstyle_h

#include "shootingstyle.h"
#include "shootingstyledefinition.h"

extern const MELShootingStyleClass MELParticuleShootingStyleClass;

MELShootingStyle * _Nonnull MELParticuleShootingStyleAlloc(const MELShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager);

#endif /* particuleshootingstyle_h */
