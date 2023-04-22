//
//  burstshootingstyle.c
//  Kuroobi
//
//  Created by Raphaël Calabro on 16/02/2023.
//

#include "burstshootingstyle.h"

#include "bulletmotion.h"
#include "animation.h"
#include "random.h"

MELShootingStyle * _Nonnull MELBurstShootingStyleAlloc(const MELShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    MELShootingStyle *self = malloc(sizeof(MELShootingStyle));
    *self = MELShootingStyleMake(&MELBurstShootingStyleClass, definition, spriteManager);
    return self;
}

static void shoot(MELShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer) {
    const MELShootingStyleDefinition *definition = self->definition;
    MELSpriteManager *spriteManager = self->spriteManager;

    MELSpriteDefinition bulletDefinition = spriteManager->definitions.memory[definition->bulletDefinition];
    bulletDefinition.type = type;

    const int damage = definition->damage;
    const int animationIndex = definition->animation;
    const float animationAngle = definition->animationAngle;

    const float bulletSpeed = definition->bulletSpeed;
    const unsigned int bulletAmount = self->bulletAmount;
    for (unsigned int index = 0; index < bulletAmount; index++) {
        const float bulletAngle = angle + MELRandomFloat(0.1) - 0.05;
        const MELPoint speed = (MELPoint) {
            .x = cosf(bulletAngle) * bulletSpeed,
            .y = sinf(bulletAngle) * bulletSpeed
        };

        MELSprite *shot = MELSpriteAlloc(spriteManager, bulletDefinition, layer);
        MELSpriteSetFrameOrigin(shot, origin);
        MELSpriteSetMotion(shot, MELBulletMotionAlloc(animationAngle + bulletAngle, speed, damage));
        if (animationIndex != 0) {
            MELSpriteSetAnimation(shot, MELAnimationAlloc(bulletDefinition.animations.memory + animationIndex));
        }
    }
}

const MELShootingStyleClass MELBurstShootingStyleClass = {
    .update = &MELShootingStyleUpdate,
    .shoot = &shoot,
    .invert = &NoShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
