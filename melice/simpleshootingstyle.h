//
//  simpleshootingstyle.h
//  melice
//
//  Created by Raphaël Calabro on 22/04/2023.
//

#ifndef simpleshootingstyle_h
#define simpleshootingstyle_h

#include "shootingstyle.h"
#include "shootingstyledefinition.h"

extern const MELShootingStyleClass MELSimpleShootingStyleClass;

MELShootingStyle * _Nonnull MELSimpleShootingStyleAlloc(const MELShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager);

#endif /* simpleshootingstyle_h */
