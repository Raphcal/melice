//
//  circularshootingstyle.c
//  shmup
//
//  Created by Raphaël Calabro on 18/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "circularshootingstyle.h"

#include "bulletmotion.h"
#include "animation.h"
#include "melmath.h"

MELShootingStyle * _Nonnull MELCircularShootingStyleAlloc(const MELShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    MELCircularShootingStyle *self = malloc(sizeof(MELCircularShootingStyle));
    *self = (MELCircularShootingStyle) {
        .super = MELShootingStyleMake(&MELCircularShootingStyleClass, definition, spriteManager),
        .baseAngle = definition->baseAngle,
        .baseAngleVariation = definition->baseAngleVariation,
    };
    return (MELShootingStyle *)self;
}

void MELCircularShootingStyleShoot(MELShootingStyle * _Nonnull shootingStyle, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer) {
    MELCircularShootingStyle *self = (MELCircularShootingStyle *)shootingStyle;
    const MELShootingStyleDefinition *definition = self->super.definition;
    MELSpriteManager *spriteManager = self->super.spriteManager;
    
    MELSpriteDefinition bulletDefinition = spriteManager->definitions.memory[definition->bulletDefinition];
    bulletDefinition.type = type;

    const int bulletAmount = self->super.bulletAmount;

    GLfloat currentAngle = angle + self->baseAngle;
    GLfloat angleIncrement = definition->angleIncrement;
    if (!angleIncrement) {
        angleIncrement = MEL_2_PI / bulletAmount;
    }

    const GLfloat bulletSpeed = definition->bulletSpeed;
    const int damage = definition->damage;
    const int animationIndex = definition->animation;
    const float animationAngle = definition->animationAngle;

    for (int index = 0; index < bulletAmount; index++) {
        MELPoint speed = MELPointMake(cosf(currentAngle) * bulletSpeed, sinf(currentAngle) * bulletSpeed);
        
        MELSprite *shot = MELSpriteAlloc(spriteManager, bulletDefinition, layer);
        MELSpriteSetFrameOrigin(shot, origin);
        MELSpriteSetMotion(shot, MELBulletMotionAlloc(animationAngle + currentAngle, speed, damage));
        if (animationIndex != 0) {
            MELSpriteSetAnimation(shot, MELAnimationAlloc(bulletDefinition.animations.memory + animationIndex));
        }

        currentAngle += angleIncrement;
    }
    
    self->baseAngle += self->baseAngleVariation;
}

void MELCircularShootingStyleInvert(MELShootingStyle * _Nonnull shootingStyle) {
    MELCircularShootingStyle *self = (MELCircularShootingStyle *)shootingStyle;
    if (self->super.definition->inversions & MELShootingStyleInversionAngle) {
        self->baseAngleVariation = -self->baseAngleVariation;
    }
}

const MELShootingStyleClass MELCircularShootingStyleClass = {
    .update = &MELShootingStyleUpdate,
    .shoot = &MELCircularShootingStyleShoot,
    .invert = &MELCircularShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
