//
//  bullet.h
//  melice
//
//  Created by Raphaël Calabro on 03/10/2026.
//

#ifndef bullet_h
#define bullet_h

#include "shootingstyle.h"

void BulletConstructor(const MELShootingStyle * _Nonnull shootingStyle, MELPoint origin, MELPoint speed, float angle, float initialDelta);

#endif /* bullet_h */
