//
//  shootingstyle.h
//  shmup
//
//  Created by Raphaël Calabro on 14/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef shootingstyle_h
#define shootingstyle_h

#include "melstd.h"

#include "shootingstyledefinition.h"
#include "sprite.h"
#include "point.h"
#include "spritemanager.h"

typedef struct shootingstyle MELShootingStyle;

typedef struct {
    void (* _Nonnull update)(MELShootingStyle * _Nonnull self, MELSprite * _Nonnull sprite, GLfloat angle, MELTimeInterval timeSinceLastUpdate);
    void (* _Nonnull shoot)(MELShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer);
    void (* _Nonnull invert)(MELShootingStyle * _Nonnull self);
    void (* _Nonnull deinit)(MELShootingStyle * _Nonnull self);
} MELShootingStyleClass;

#define MELShootingStyleClassDefaults .update = &MELShootingStyleUpdate, .shoot = &NoShootingStyleShoot, .invert = &NoShootingStyleInvert, .deinit = &NoShootingStyleDeinit

struct shootingstyle {
    const MELShootingStyleClass * _Nonnull class;
    const MELShootingStyleDefinition * _Nonnull definition;
    MELSpriteManager * _Nonnull spriteManager;
    
    MELTimeInterval shootInterval;
    int bulletAmount;
    int bulletAmountVariation;
    
    int inversionInterval;
};

MELShootingStyle MELShootingStyleMake(const MELShootingStyleClass * _Nonnull class, const MELShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager);

/**
 * Fait tirer le sprite donné dans la direction donnée (généralement -pi/2 pour tirer vers le haut ou pi/2 pour tirer vers le bas).
 *
 * @param self Instance du style de tir.
 * @param sprite Tireur.
 * @param angle Direction dans laquelle sont dirigés les tirs.
 * @param timeSinceLastUpdate Temps écoulé depuis le dernier appel.
 */
void MELShootingStyleUpdate(MELShootingStyle * _Nonnull self, MELSprite * _Nonnull sprite, GLfloat angle, MELTimeInterval timeSinceLastUpdate);

/**
 * Ne tire pas.
 */
void NoShootingStyleShoot(MELShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer);

/**
 * Ne fait aucune inversion.
 */
void NoShootingStyleInvert(MELShootingStyle * _Nonnull self);

/**
 * Ne fait aucun traitement.
 */
void NoShootingStyleDeinit(MELShootingStyle * _Nonnull self);

#endif /* shootingstyle_h */
