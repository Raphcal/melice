//
//  aimedshootingstyle.c
//  Kuroobi
//
//  Created by Raphaël Calabro on 10/02/2023.
//

#include "aimedshootingstyle.h"

#include "bullet.h"
#include "random.h"

static void createBullets(MELShootingStyle * _Nonnull self, MELPoint origin, float angle, float initialDelta);

static const MELShootingStyleClass AimedShootingStyleClass = (MELShootingStyleClass) {
    .name = MELShootingStyleClassNameAimed,
    .createBullets = createBullets,
};

const MELShootingStyleClass * _Nonnull AimedShootingStyleGetClass(void) {
    return &AimedShootingStyleClass;
}

void AimedShootingStyleInit(MELShootingStyle * _Nonnull self, const MELShootingStyleDefinition * _Nonnull definition) {
    *self = (MELShootingStyle) {
        .class = &AimedShootingStyleClass,
        .definition = definition,
    };
    MELShootingStyleInit(self);
}

static void createBullets(MELShootingStyle * _Nonnull self, MELPoint origin, float angle, float initialDelta) {
    const MELShootingStyleDefinition *definition = (MELShootingStyleDefinition *)self->definition;

    MELSprite *melTarget = MELShootingStyleGetTarget(self);
    if (melTarget == NULL) {
        return;
    }

    const float bulletSpeed = definition->bulletSpeed;
    const unsigned int bulletAmount = self->bulletAmount;
    for (unsigned int index = 0; index < bulletAmount; index++) {
        const float angleToTarget = MELPointAngleToPoint(melTarget->frame.origin, origin);

        const MELPoint speed = (MELPoint) {
            .x = cosf(angleToTarget) * bulletSpeed,
            .y = sinf(angleToTarget) * bulletSpeed
        };
        BulletConstructor(self, origin, speed, angleToTarget, initialDelta);
    }
}
