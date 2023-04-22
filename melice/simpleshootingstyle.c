//
//  simpleshootingstyle.c
//  melice
//
//  Created by Raphaël Calabro on 22/04/2023.
//

#include "simpleshootingstyle.h"

#include "bulletmotion.h"
#include "animation.h"

MELShootingStyle * _Nonnull MELSimpleShootingStyleAlloc(const MELShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    MELShootingStyle *self = malloc(sizeof(MELShootingStyle));
    *self = MELShootingStyleMake(&MELSimpleShootingStyleClass, definition, spriteManager);
    return self;
}

static void MELSimpleShootingStyleShoot(MELShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer) {
    const MELShootingStyleDefinition *definition = self->definition;
    MELSpriteManager *spriteManager = self->spriteManager;

    MELSpriteDefinition bulletDefinition = spriteManager->definitions.memory[definition->bulletDefinition];
    bulletDefinition.type = type;

    const float bulletSpeed = definition->bulletSpeed;
    const int damage = definition->damage;
    const int animationIndex = definition->animation;
    const float animationAngle = definition->animationAngle;

    MELSprite *shot = MELSpriteAlloc(spriteManager, bulletDefinition, layer);
    MELSpriteSetFrameOrigin(shot, origin);
    MELSpriteSetMotion(shot, MELBulletMotionAlloc(animationAngle + angle, (MELPoint) {
        .x = bulletSpeed * cosf(angle),
        .y = bulletSpeed * sinf(angle)
    }, damage));
    if (animationIndex != 0) {
        MELSpriteSetAnimation(shot, MELAnimationAlloc(bulletDefinition.animations.memory + animationIndex));
    }
}

const MELShootingStyleClass MELSimpleShootingStyleClass = {
    .update = &MELShootingStyleUpdate,
    .shoot = &MELSimpleShootingStyleShoot,
    .invert = &NoShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
