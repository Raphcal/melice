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

typedef struct shootingstyledefinition {
    /// Origin of the shots.
    MELShotOrigin origin;

    /// Damage made by each bullet.
    int damage;

    /// Sprite index inside the atlas.
    int bulletDefinition;
    /// Animation index inside the sprite definition.
    int animation;
    /// Base angle of the animation. Will be used to rotate the bullets.
    GLfloat animationAngle;

    /// Number of bullets shot at once.
    int bulletAmount;
    /// Bullet count increment or decrement at each variation.
    int bulletAmountVariation;

    /// Distance in pixel travelled by each bullet in one second.
    GLfloat bulletSpeed;
    /// Time interval between each shot.
    MELTimeInterval shootInterval;

    /// Inversions.
    MELShootingStyleInversion inversions;
    /// Number of shots before an inversion occurs.
    int inversionInterval;

    /// For aimed shooting style: target type.
    MELSpriteType targetType;

    /// For circular shooting style: base angle of the first shot.
    GLfloat baseAngle;

    /// For circular shooting style: value added to the base angle when a variation occurs.
    GLfloat baseAngleVariation;

    /// For circular shooting style: when more than one shot is fired, angle difference between one shot and the next one. If zero, it will equals 2π divided by the number of bullets.
    GLfloat angleIncrement;

    /// For straight shooting style: space between each bullet.
    GLfloat space;

    /// For straight shooting style: translation added to the firing sprite location.
    MELPoint translation;
} MELShootingStyleDefinition;

#define MELShootingStyleDefinitionDefaults .origin = MELShotOriginFront, .damage = 1, .bulletAmount = 1, .bulletAmountVariation = 0, .bulletSpeed = 500, .shootInterval = 0.1, .inversions = 0, .inversionInterval = 0, .bulletDefinition = 0

#endif /* shootingstyledefinition_h */
