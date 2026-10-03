//
//  bullet.c
//  melice
//
//  Created by Raphaël Calabro on 03/10/2026.
//

#include "bullet.h"

#include "bulletmotion.h"
#include "animation.h"

void BulletConstructor(const MELShootingStyle * _Nonnull shootingStyle, MELPoint origin, MELPoint speed, float angle, float initialDelta) {
    MELSpriteManager *spriteManager = shootingStyle->spriteManager;
    const MELShootingStyleDefinition *definition = shootingStyle->definition;
    MELSpriteDefinition bulletDefinition = spriteManager->definitions.memory[definition->bulletDefinition];
    bulletDefinition.type = shootingStyle->type;

    MELSprite *shot = MELSpriteAlloc(shootingStyle->spriteManager, bulletDefinition, shootingStyle->layer);
    MELSpriteSetFrameOrigin(shot, origin);

    /// NOTE: Pas besoin de faire la rotation à `definition->animationAngle + angle` car le code ci-après le fait de la même façon que pour Playdate.
    MELSpriteSetMotion(shot, MELBulletMotionAlloc(0, speed, definition->damage));

    MELAnimation *animation;
    if (definition->animation == 0) {
        animation = shot->animation;
    } else {
        animation = MELAnimationAlloc(bulletDefinition.animations.memory + definition->animation);
        MELSpriteSetAnimation(shot, animation);
    }
    if (animation->definition->type == MELAnimationTypePlayOnce) {
        const unsigned int frameCount = animation->definition->frameCount;
        const unsigned int frameIndex = (int)ceilf(frameCount * ((MEL_2_PI + angle) / MEL_2_PI)) % frameCount;
        MELSingleFrameAnimationReuse(animation, frameIndex);
    }
}
