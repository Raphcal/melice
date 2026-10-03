//
//  shootingstyle.c
//  Kuroobi
//
//  Created by Raphaël Calabro on 27/01/2023.
//

#include "shootingstyle.h"

#include "burstshootingstyle.h"
#include "circularshootingstyle.h"
#include "simpleshootingstyle.h"
#include "particuleshootingstyle.h"
#include "sprite.h"
#include "melmath.h"
#include "random.h"

const MELShootingStyle MELShootingStyleEmpty = (MELShootingStyle) {};

static MELPoint shotOrigin(MELShotOrigin origin, MELRectangle frame, float angle) {
    switch (origin) {
        case MELShotOriginFront:
            return MELRectangleOriginIsCenterGetPointAtAngle(frame, angle);
        case MELShotOriginCenter:
            return frame.origin;
        case MELShotOriginBack:
            return MELRectangleOriginIsCenterGetPointAtAngle(frame, angle + MEL_PI);
        default:
            return frame.origin;
    }
}

void MELShootingStyleInit(MELShootingStyle * _Nonnull self) {
    const MELShootingStyleDefinition *definition = self->definition;
    self->shootInterval = MELRandomFloat(definition->shootInterval);
    self->bulletAmount = definition->bulletAmount;
    self->bulletAmountVariation = definition->bulletAmountVariation;
    self->inversionInterval = definition->inversionInterval;
    self->shotsBeforePause = definition->pauseAfterShots;
}

#if ENABLE_SHOOTING_STYLE_INVERSIONS
static void invert(MELShootingStyle * _Nonnull self, const MELShootingStyleInversion inversions) {
    if (inversions & MELShootingStyleInversionAmount) {
        self->bulletAmountVariation = -self->bulletAmountVariation;
    }
}
#endif

void MELShootingStyleShootFromSprite(MELShootingStyle * _Nonnull self, MELSprite * _Nonnull sprite, float angle, MELTimeInterval timeSinceLastUpdate) {
    MELTimeInterval shootInterval = self->shootInterval - timeSinceLastUpdate;
    while (shootInterval <= 0) {
        const float initialDelta = -shootInterval;
        const MELShootingStyleDefinition *definition = self->definition;

        if (!definition->pauseDuration || self->shotsBeforePause > 0) {
            shootInterval += MELFloatMax(definition->shootInterval, 0.01f);
            self->shotsBeforePause -= definition->pauseDuration > 0;
        } else if (self->shotsBeforePause == 0) {
            shootInterval += MELFloatMax(definition->pauseDuration, 0.01f);
            self->shotsBeforePause = definition->pauseAfterShots;
        }

        MELPoint origin = shotOrigin(definition->origin, sprite->frame, angle);
        const MELPoint translation = definition->translation;
        origin = (MELPoint) {
            .x = origin.x + translation.x,
            .y = origin.y + translation.y
        };

        // Salve de tir
        self->spriteManager = sprite->parent;
        self->layer = sprite->layer;
        self->type = sprite->definition.type == MELSpriteTypePlayer ? MELSpriteTypeFriendlyShot : MELSpriteTypeEnemyShot;
        self->class->createBullets(self, origin, angle, initialDelta);

        self->bulletAmount += definition->bulletAmountVariation;

#if ENABLE_SHOOTING_STYLE_INVERSIONS
        const MELShootingStyleInversion inversions = definition->inversions;
        if (inversions && self->inversionInterval > 0) {
            self->inversionInterval--;
        } else if (inversions) {
            self->inversionInterval = definition->inversionInterval;
            invert(self, inversions);
        }
#endif
    }
    self->shootInterval = shootInterval;
}

MELSprite * _Nullable MELShootingStyleGetTarget(const MELShootingStyle * _Nonnull self) {
    const MELShootingStyleDefinition *definition = self->definition;
    if (!definition->aimed) {
        return NULL;
    }
    MELSpriteManager *spriteManager = self->spriteManager;
    // TODO: Filtrer le groupe pour ne prendre que les sprites dont la type est TargetType car plusieurs types peuvent être dans le même groupe.
    MELSpriteRefList sprites = spriteManager->groups[spriteManager->groupForType[definition->targetType]];
    if (sprites.count > 0) {
        return sprites.memory[MELRandomInt((int)sprites.count)];
    } else {
        return NULL;
    }
}

const MELShootingStyleClass * _Nullable MELShootingStyleClassForName(MELShootingStyleClassName className) {
    switch (className) {
        case MELShootingStyleClassNameBurst:
            return BurstShootingStyleGetClass();
        case MELShootingStyleClassNameCircular:
            return CircularShootingStyleGetClass();
        case MELShootingStyleClassNameSimple:
            return SimpleShootingStyleGetClass();
        case MELShootingStyleClassNameParticule:
            return ParticuleShootingStyleGetClass();
        default:
            return NULL;
    }
}
