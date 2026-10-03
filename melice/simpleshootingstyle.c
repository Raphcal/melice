//
//  simpleshootingstyle.c
//  Kuroobi
//
//  Created by Raphaël Calabro on 21/03/2023.
//

#include "simpleshootingstyle.h"

#include "bullet.h"
#include "melmath.h"
#include "random.h"

static void createBullets(MELShootingStyle * _Nonnull self, MELPoint origin, float angle, float initialDelta);

static const MELShootingStyleClass SimpleShootingStyleClass = (MELShootingStyleClass) {
    .name = MELShootingStyleClassNameSimple,
    .createBullets = createBullets,
};

const MELShootingStyleClass * _Nonnull SimpleShootingStyleGetClass(void) {
    return &SimpleShootingStyleClass;
}

void SimpleShootingStyleInit(MELShootingStyle * _Nonnull self, const MELShootingStyleDefinition * _Nonnull definition) {
    *self = (MELShootingStyle) {
        .class = &SimpleShootingStyleClass,
        .definition = definition,
    };
    MELShootingStyleInit(self);
}

static void createBullets(MELShootingStyle * _Nonnull self, MELPoint origin, float angle, float initialDelta) {
    const MELShootingStyleDefinition *definition = self->definition;
    const float bulletSpeed = definition->bulletSpeed;
    MELSprite *melTarget = MELShootingStyleGetTarget(self);
    if (melTarget) {
        angle = MELPointAngleToPoint(melTarget->frame.origin, origin);
    }
    BulletConstructor(self, origin, (MELPoint) {
        .x = bulletSpeed * cosf(angle),
        .y = bulletSpeed * sinf(angle)
    }, angle, initialDelta);
}
