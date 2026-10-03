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

#include "point.h"
#include "spritedefinition.h"
#include "melmath.h"

// TODO: Retirer les inversions inutilisées à la fin.
typedef enum {
    MELShootingStyleNone            =       0,
    MELShootingStyleInversionX      =       1,
    MELShootingStyleInversionY      =    0b10,
    MELShootingStyleInversionAim    =   0b100,
    MELShootingStyleInversionAngle  =  0b1000,
    MELShootingStyleInversionAmount = 0b10000,
} MELShootingStyleInversion;

typedef enum {
    MELShotOriginCenter,
    MELShotOriginFront,
    MELShotOriginBack,
} MELShotOrigin;

typedef struct shootingstyle MELShootingStyle;
typedef struct shootingstyledefinition MELShootingStyleDefinition;

typedef struct shootingstyledefinition {
    /// Origin of the shots.
    MELShotOrigin origin;

    /// For straight shooting style: translation added to the firing sprite location.
    MELPoint translation;

    /// Mouvement des tirs.
    MELPoint speeds;

    /// Si non null, tire en direction de la cible donnée.
    MELSprite * _Nullable (* _Nullable getTarget)(void * _Nullable userdata);
    MELBoolean aimed;

    /// Fonction pour le mouvement des tirs.
    MELEasingFunction easingFunction;

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
    float bulletSpeed;
    /// Time interval between each shot.
    MELTimeInterval shootInterval;

    /// Nombre de tirs avant une pause.
    int pauseAfterShots;
    /// Durée de la pause.
    MELTimeInterval pauseDuration;
    
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
} MELShootingStyleDefinition;

#endif /* shootingstyledefinition_h */
