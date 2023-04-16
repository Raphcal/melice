//
//  shootingstyledefinition.h
//  shmup
//
//  Created by Raphaël Calabro on 14/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef shootingstyledefinition_h
#define shootingstyledefinition_h

#include "melstd.h"

#include "spritemanager.h"

#define ShootingStyleInversionX 1
#define ShootingStyleInversionY 2
#define ShootingStyleInversionAim 4
#define ShootingStyleInversionAngle 8
#define ShootingStyleInversionAmount 16

typedef int ShootingStyleInversion;

typedef enum {
    ShotOriginCenter,
    ShotOriginFront
} ShotOrigin;

typedef struct shootingstyle ShootingStyle;
typedef struct shootingstyledefinition ShootingStyleDefinition;

typedef ShootingStyle * _Nonnull (* _Nonnull ShootingStyleDefinitionShootingStyleAlloc)(const ShootingStyleDefinition * _Nonnull self, MELSpriteManager * _Nonnull spriteManager);

typedef struct shootingstyledefinition {
    ShootingStyleDefinitionShootingStyleAlloc shootingStyleAlloc;
    
    /// Point d'origine des tirs
    ShotOrigin origin;
    
    /// Dommages provoqués par chaque tir
    int damage;
    
    /// Nombre de tirs
    int shotAmount;
    /// Augmentation ou diminution du nombre de tirs
    const int shotAmountVariation;
    
    /// Vitesse d'un tir
    GLfloat shotSpeed;
    /// Interval de temps entre chaque tir
    MELTimeInterval shootInterval;
    
    /// Nombre d'inversions
    ShootingStyleInversion inversions;
    /// Nombre de tirs avant l'inversion
    const int inversionInterval;
    
    /// Numéro du sprite dans l'atlas
    int spriteDefinition;
} ShootingStyleDefinition;

#define ShootingStyleDefinitionDefaults .origin = ShotOriginFront, .damage = 1, .shotAmount = 1, .shotAmountVariation = 0, .shotSpeed = 500, .shootInterval = 0.1, .inversions = 0, .inversionInterval = 0, .spriteDefinition = 0

#endif /* shootingstyledefinition_h */
