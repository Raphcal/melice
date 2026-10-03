//
//  sinussimpleshootingstyle.c
//  ColdBird
//
//  Created by Raphaël Calabro on 29/01/2025.
//

#include "sinussimpleshootingstyle.h"

#include "bullet.h"
#include "random.h"
#include "meltime.h"

static void createBullets(MELShootingStyle * _Nonnull self, MELPoint origin, float angle, float initialDelta);

static const MELShootingStyleClass SinusShootingStyleClass = (MELShootingStyleClass) {
    .name = MELShootingStyleClassNameSinus,
    .createBullets = createBullets,
};

const MELShootingStyleClass * _Nonnull SinusShootingStyleGetClass(void) {
    return &SinusShootingStyleClass;
}

void SinusShootingStyleInit(MELShootingStyle * _Nonnull self, const MELShootingStyleDefinition * _Nonnull definition) {
    *self = (MELShootingStyle) {
        .class = &SinusShootingStyleClass,
        .definition = definition,
    };
    MELShootingStyleInit(self);
}

static void createBullets(MELShootingStyle * _Nonnull self, MELPoint origin, float angle, float initialDelta) {
    const MELShootingStyleDefinition *definition = self->definition;
    int64_t time = MELMilliTime() % 100000;
    const float progress = sinf((time * definition->speeds.x) / 1000.0f);
    printf("time: %lld, speeds.x: %f, progress: %f", time, definition->speeds.x, progress);
    BulletConstructor(self,
                      MELPointAdd(origin, MELPointMake(progress * definition->space, 0.0f)),
                      MELPointMake(0.0f, definition->speeds.y * definition->bulletSpeed),
                      angle, initialDelta);
    BulletConstructor(self,
                      MELPointAdd(origin, MELPointMake(-progress * definition->space, 0.0f)),
                      MELPointMake(0.0f, definition->speeds.y * definition->bulletSpeed),
                      angle, initialDelta);
}
