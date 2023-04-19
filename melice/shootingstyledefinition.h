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

#define MELShootingStyleInversionX 1
#define MELShootingStyleInversionY 2
#define MELShootingStyleInversionAim 4
#define MELShootingStyleInversionAngle 8
#define MELShootingStyleInversionAmount 16

typedef int MELShootingStyleInversion;

typedef enum {
    MELShotOriginCenter,
    MELShotOriginFront
} MELShotOrigin;

typedef enum {
    MELShootingStyleTypeAimed,
    MELShootingStyleTypeCircular,
    MELShootingStyleTypeStraight,
} MELShootingStyleType;

typedef struct shootingstyle MELShootingStyle;
typedef struct shootingstyledefinition MELShootingStyleDefinition;

typedef MELShootingStyle * _Nonnull (* _Nonnull MELShootingStyleDefinitionShootingStyleAlloc)(const MELShootingStyleDefinition * _Nonnull self, MELSpriteManager * _Nonnull spriteManager);

typedef struct shootingstyledefinition {
    MELShootingStyleDefinitionShootingStyleAlloc shootingStyleAlloc;

    /// Point d'origine des tirs
    MELShotOrigin origin;

    /// Dommages provoqués par chaque tir
    int damage;

    /// Nombre de tirs
    int bulletAmount;
    /// Augmentation ou diminution du nombre de tirs
    int bulletAmountVariation;

    /// Vitesse d'un tir
    GLfloat bulletSpeed;
    /// Interval de temps entre chaque tir
    MELTimeInterval shootInterval;

    /// Nombre d'inversions
    MELShootingStyleInversion inversions;
    /// Nombre de tirs avant l'inversion
    int inversionInterval;

    /// Numéro du sprite dans l'atlas
    int bulletDefinition;
} MELShootingStyleDefinition;

#define MELShootingStyleDefinitionDefaults .origin = MELShotOriginFront, .damage = 1, .bulletAmount = 1, .bulletAmountVariation = 0, .bulletSpeed = 500, .shootInterval = 0.1, .inversions = 0, .inversionInterval = 0, .bulletDefinition = 0

#endif /* shootingstyledefinition_h */
