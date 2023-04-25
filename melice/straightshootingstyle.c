//
//  straightshootingstyle.c
//  shmup
//
//  Created by Raphaël Calabro on 16/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "straightshootingstyle.h"

#include "bulletmotion.h"
#include "animation.h"

MELShootingStyle * _Nonnull MELStraightShootingStyleAlloc(const MELShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    MELShootingStyle *self = malloc(sizeof(MELShootingStyle));
    *self = MELShootingStyleMake(&MELStraightShootingStyleClass, definition, spriteManager);
    return self;
}

static void MELStraightShootingStyleShoot(MELShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer) {
    const MELShootingStyleDefinition *definition = self->definition;
    MELSpriteManager *spriteManager = self->spriteManager;
    
    MELSpriteDefinition bulletDefinition = spriteManager->definitions.memory[definition->bulletDefinition];
    bulletDefinition.type = type;
    
    const GLfloat bulletSpeed = definition->bulletSpeed;
    const int damage = definition->damage;
    const int animationIndex = definition->animation;
    const float animationAngle = definition->animationAngle;
    const GLfloat space = definition->space;

    const int bulletAmount = self->bulletAmount;

    GLfloat left = origin.x - (((GLfloat)bulletAmount - 1) * space) / 2;

    for (int index = 0; index < bulletAmount; index++) {
        MELPoint speed = MELPointMake(cosf(angle) * bulletSpeed, sinf(angle) * bulletSpeed);
        MELSprite *shot = MELSpriteAlloc(spriteManager, bulletDefinition, layer);
        MELSpriteSetFrameOrigin(shot, MELPointMake(left, origin.y));
        MELSpriteSetMotion(shot, MELBulletMotionAlloc(animationAngle + angle, speed, damage));
        if (animationIndex != 0) {
            MELSpriteSetAnimation(shot, MELAnimationAlloc(bulletDefinition.animations.memory + animationIndex));
        }

        left += space;
    }
}

const MELShootingStyleClass MELStraightShootingStyleClass = {
    .update = &MELShootingStyleUpdate,
    .shoot = &MELStraightShootingStyleShoot,
    .invert = &NoShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
