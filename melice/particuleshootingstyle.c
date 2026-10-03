//
//  particuleshootingstyle.c
//  Kuroobi
//
//  Created by Raphaël Calabro on 22/04/2023.
//

#include "particuleshootingstyle.h"

#include "particulemotion.h"
#include "animation.h"
#include "random.h"

static void createBullets(MELShootingStyle * _Nonnull self, MELPoint origin, float angle, float initialDelta);

static const MELShootingStyleClass ParticuleShootingStyleClass = (MELShootingStyleClass) {
    .name = MELShootingStyleClassNameParticule,
    .createBullets = createBullets,
};

const MELShootingStyleClass * _Nonnull ParticuleShootingStyleGetClass(void) {
    return &ParticuleShootingStyleClass;
}

void ParticuleShootingStyleInit(MELShootingStyle * _Nonnull self, const MELShootingStyleDefinition * _Nonnull definition) {
    *self = (MELShootingStyle) {
        .class = &ParticuleShootingStyleClass,
        .definition = definition,
        .shootInterval = MELRandomFloat(definition->shootInterval),
        .canShootWhenHitPointsAreZero = true,
    };
    MELShootingStyleInit(self);
}

static void createBullets(MELShootingStyle * _Nonnull self, MELPoint origin, float angle, float initialDelta) {
    const MELShootingStyleDefinition *definition = self->definition;
    MELSpriteManager *spriteManager = self->spriteManager;

    MELSpriteDefinition bulletDefinition = spriteManager->definitions.memory[definition->bulletDefinition];
    bulletDefinition.type = MELSpriteTypeDecor;

    const int animationIndex = definition->animation;
    const float space = definition->space;
    const float halfSpace = space / 2.0f;

    const int bulletAmount = self->bulletAmount;
    for (int index = 0; index < bulletAmount; index++) {
        MELSprite *particule = MELSpriteAlloc(spriteManager, bulletDefinition, self->layer);
        MELSpriteSetFrameOrigin(particule, (MELPoint) {
            .x = origin.x + MELRandomFloat(space) - halfSpace,
            .y = origin.y + MELRandomFloat(space) - halfSpace,
        });
        MELSpriteSetMotion(particule, MELParticuleMotionAlloc());
        if (animationIndex != 0) {
            MELSpriteSetAnimation(particule, MELAnimationAlloc(bulletDefinition.animations.memory + animationIndex));
        }
    }
}
