//
//  circularshootingstyle.c
//  Kuroobi
//
//  Created by Raphaël Calabro on 10/02/2023.
//

#include "circularshootingstyle.h"

#include "bullet.h"
#include "melmath.h"
#include "random.h"

static void createBullets(MELShootingStyle * _Nonnull self, MELPoint origin, float angle, float initialDelta);

static const MELShootingStyleClass CircularShootingStyleClass = (MELShootingStyleClass) {
    .name = MELShootingStyleClassNameCircular,
    .createBullets = createBullets,
};

const MELShootingStyleClass * _Nonnull CircularShootingStyleGetClass(void) {
    return &CircularShootingStyleClass;
}

void CircularShootingStyleInit(MELShootingStyle * _Nonnull self, const MELShootingStyleDefinition * _Nonnull definition) {
    *self = (MELShootingStyle) {
        .class = &CircularShootingStyleClass,
        .definition = definition,
        .baseAngle = definition->baseAngle,
    };
    MELShootingStyleInit(self);
}

static void createBullets(MELShootingStyle * _Nonnull self, MELPoint origin, float angle, float initialDelta) {
    const MELShootingStyleDefinition *definition = self->definition;

    const float bulletSpeed = definition->bulletSpeed;
    const float baseAngle = self->baseAngle;
    self->baseAngle = baseAngle + definition->baseAngleVariation;
    angle += baseAngle;

    const unsigned int bulletAmount = self->bulletAmount;
    float angleIncrement = definition->angleIncrement;
    if (!angleIncrement) {
        angleIncrement = MEL_2_PI / bulletAmount;
    }

    MELSprite *melTarget = MELShootingStyleGetTarget(self);
    if (melTarget == NULL) {
        for (unsigned int index = 0; index < bulletAmount; index++) {
            const float cosAngle = cosf(angle);
            const float sinAngle = sinf(angle);
            BulletConstructor(self, (MELPoint) {
                .x = origin.x + cosAngle * definition->space,
                .y = origin.y + sinAngle * definition->space,
            }, (MELPoint) {
                .x = cosAngle * bulletSpeed,
                .y = sinAngle * bulletSpeed,
            }, angle, initialDelta);
            angle += angleIncrement;
        }
    } else {
        const float angleToTarget = MELPointAngleToPoint(melTarget->frame.origin, origin);
        const MELPoint speed = (MELPoint) {
            .x = cosf(angleToTarget) * bulletSpeed,
            .y = sinf(angleToTarget) * bulletSpeed
        };
        for (unsigned int index = 0; index < bulletAmount; index++) {
            BulletConstructor(self, (MELPoint) {
                .x = origin.x + cosf(angle) * definition->space,
                .y = origin.y + sinf(angle) * definition->space,
            }, speed, angleToTarget, initialDelta);
            angle += angleIncrement;
        }
    }
}
