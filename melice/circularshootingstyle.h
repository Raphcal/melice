//
//  circularshootingstyle.h
//  Kuroobi
//
//  Created by Raphaël Calabro on 10/02/2023.
//

#ifndef circularshootingstyle_h
#define circularshootingstyle_h

#include "shootingstyle.h"
#include "shootingstyledefinition.h"

const MELShootingStyleClass * _Nonnull CircularShootingStyleGetClass(void);

void CircularShootingStyleInit(MELShootingStyle * _Nonnull self, const MELShootingStyleDefinition * _Nonnull definition);

#endif /* circularshootingstyle_h */
