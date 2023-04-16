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

MELPoint MELRectanglePointAtAngle(MELRectangle self, GLfloat angle);

ShootingStyle ShootingStyleMake(const ShootingStyleClass * _Nonnull class, const ShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    return (ShootingStyle) {
        class,
        definition,
        spriteManager,
        MELRandomTimeIntervalWithRange(0, definition->shootInterval),
        definition->shotAmount,
        definition->shotAmountVariation,
        definition->inversionInterval
    };
}

void ShootingStyleInvert(ShootingStyle * _Nonnull self) {
    if (self->definition->inversions & ShootingStyleInversionAmount) {
        self->shotAmountVariation = -self->shotAmountVariation;
    }
    self->class->invert(self);
}

void ShootingStyleUpdate(ShootingStyle * _Nonnull self, MELSprite * _Nonnull sprite, GLfloat angle, MELTimeInterval timeSinceLastUpdate) {
    MELTimeInterval shootInterval = self->shootInterval;
    if (shootInterval > 0) {
        shootInterval -= timeSinceLastUpdate;
    } else {
        shootInterval += self->definition->shootInterval;
        
        const ShootingStyleDefinition *definition = self->definition;
        
        MELPoint origin;
        switch (definition->origin) {
            case ShotOriginFront:
                origin = MELRectanglePointAtAngle(sprite->frame, angle);
                break;
            default:
                origin = sprite->frame.origin;
                break;
        }
        
        // Salve de tir
        self->class->shoot(self, origin, angle, sprite->definition.type == MELSpriteTypePlayer ? MELSpriteTypeFriendlyShot : MELSpriteTypeEnemyShot, sprite->layer);
        
        self->shotAmount += self->shotAmountVariation;
        
        if (definition->inversions != 0) {
            int inversionInterval = self->inversionInterval;
            if (inversionInterval > 0) {
                inversionInterval--;
            } else {
                inversionInterval = definition->inversionInterval;
                ShootingStyleInvert(self);
            }
            self->inversionInterval = inversionInterval;
        }
    }
    self->shootInterval = shootInterval;
}

void NoShootingStyleShoot(ShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer) {
    // Aucune action.
}

void NoShootingStyleInvert(ShootingStyle * _Nonnull self) {
    // Pas d'inversion.
}

void NoShootingStyleDeinit(ShootingStyle * _Nonnull self) {
    // Rien à libérer.
}

MELPoint MELRectanglePointAtAngle(MELRectangle self, GLfloat angle) {
    return MELPointMake(self.origin.x + self.size.width / 2 * cosf(angle), self.origin.y + self.size.height / 2 * sinf(angle));
}
