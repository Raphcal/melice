//
//  shootingstyle.c
//  shmup
//
//  Created by Raphaël Calabro on 14/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "shootingstyle.h"

#include <math.h>
#include "random.h"
#include "melmath.h"

MELPoint MELRectanglePointAtAngle(MELRectangle self, GLfloat angle);

MELShootingStyle MELShootingStyleMake(const MELShootingStyleClass * _Nonnull class, const MELShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    return (MELShootingStyle) {
        .class = class,
        .definition = definition,
        .spriteManager = spriteManager,
        .shootInterval = MELRandomTimeIntervalWithRange(0, definition->shootInterval),
        .bulletAmount = definition->bulletAmount,
        .bulletAmountVariation = definition->bulletAmountVariation,
        .inversionInterval = definition->inversionInterval,
    };
}

void MELShootingStyleInvert(MELShootingStyle * _Nonnull self) {
    if (self->definition->inversions & MELShootingStyleInversionAmount) {
        self->bulletAmountVariation = -self->bulletAmountVariation;
    }
    self->class->invert(self);
}

void MELShootingStyleUpdate(MELShootingStyle * _Nonnull self, MELSprite * _Nonnull sprite, GLfloat angle, MELTimeInterval timeSinceLastUpdate) {
    MELTimeInterval shootInterval = self->shootInterval;
    if (shootInterval > 0) {
        shootInterval -= timeSinceLastUpdate;
    } else {
        shootInterval += self->definition->shootInterval;
        
        const MELShootingStyleDefinition *definition = self->definition;
        
        MELPoint origin;
        switch (definition->origin) {
            case MELShotOriginFront:
                origin = MELRectanglePointAtAngle(sprite->frame, angle);
                break;
            case MELShotOriginBack:
                origin = MELRectanglePointAtAngle(sprite->frame, angle + MEL_PI);
                break;
            default:
                origin = sprite->frame.origin;
                break;
        }

        const MELPoint translation = definition->translation;
        origin = (MELPoint) {
            .x = origin.x + translation.x,
            .y = origin.y + translation.y
        };

        // Salve de tir
        self->class->shoot(self, origin, angle, sprite->definition.type == MELSpriteTypePlayer ? MELSpriteTypeFriendlyShot : MELSpriteTypeEnemyShot, sprite->layer);
        
        self->bulletAmount += self->bulletAmountVariation;
        
        if (definition->inversions != 0) {
            int inversionInterval = self->inversionInterval;
            if (inversionInterval > 0) {
                inversionInterval--;
            } else {
                inversionInterval = definition->inversionInterval;
                MELShootingStyleInvert(self);
            }
            self->inversionInterval = inversionInterval;
        }
    }
    self->shootInterval = shootInterval;
}

void NoShootingStyleShoot(MELShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer) {
    // Aucune action.
}

void NoShootingStyleInvert(MELShootingStyle * _Nonnull self) {
    // Pas d'inversion.
}

void NoShootingStyleDeinit(MELShootingStyle * _Nonnull self) {
    // Rien à libérer.
}

MELPoint MELRectanglePointAtAngle(MELRectangle self, GLfloat angle) {
    return MELPointMake(self.origin.x + self.size.width / 2 * cosf(angle), self.origin.y + self.size.height / 2 * sinf(angle));
}
