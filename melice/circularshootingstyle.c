//
//  circularshootingstyle.c
//  shmup
//
//  Created by Raphaël Calabro on 18/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "circularshootingstyle.h"

#include "shotmotion.h"

MELCircularShootingStyleDefinition * _Nonnull MELCircularShootingStyleDefinitionAlloc(void) {
    MELCircularShootingStyleDefinition value = (MELCircularShootingStyleDefinition) {
        {
            MELShootingStyleDefinitionDefaults,
            .shootingStyleAlloc = (MELShootingStyleDefinitionShootingStyleAlloc) MELCircularShootingStyleAlloc
        },
        0,
        0,
        0
    };
    MELCircularShootingStyleDefinition *self = malloc(sizeof(MELCircularShootingStyleDefinition));
    memcpy(self, &value, sizeof(MELCircularShootingStyleDefinition));
    return self;
}

MELShootingStyle * _Nonnull MELCircularShootingStyleAlloc(const MELCircularShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    MELCircularShootingStyle *self = malloc(sizeof(MELCircularShootingStyle));
    *self = (MELCircularShootingStyle) {
        MELShootingStyleMake(&MELCircularShootingStyleClass, (const MELShootingStyleDefinition *)definition, spriteManager),
        definition->baseAngle,
        definition->baseAngleVariation
    };
    return (MELShootingStyle *)self;
}

void MELCircularShootingStyleShoot(MELCircularShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer) {
    const MELCircularShootingStyleDefinition *definition = (const MELCircularShootingStyleDefinition *) self->super.definition;
    MELSpriteManager *spriteManager = self->super.spriteManager;
    
    MELSpriteDefinition bulletDefinition = spriteManager->definitions.memory[definition->super.bulletDefinition];
    bulletDefinition.type = type;
    
    const int bulletAmount = self->super.bulletAmount;
    
    GLfloat currentAngle = angle + definition->baseAngle;
    GLfloat angleIncrement = definition->angleIncrement;
    if (!angleIncrement) {
        angleIncrement = MEL_2_PI / bulletAmount;
    }
    
    const GLfloat bulletSpeed = definition->super.bulletSpeed;
    const int damage = definition->super.damage;
    
    for (int index = 0; index < bulletAmount; index++) {
        MELPoint speed = MELPointMake(cosf(currentAngle) * bulletSpeed, sinf(currentAngle) * bulletSpeed);
        
        MELSprite *shot = MELSpriteAlloc(spriteManager, bulletDefinition, layer);
        MELSpriteSetFrameOrigin(shot, origin);
        MELSpriteSetMotion(shot, MELBulletMotionAlloc(currentAngle, speed, damage));
        
        currentAngle += angleIncrement;
    }
    
    self->baseAngle += self->baseAngleVariation;
}

void MELCircularShootingStyleInvert(MELCircularShootingStyle * _Nonnull self) {
    if (self->super.definition->inversions & MELShootingStyleInversionAngle) {
        self->baseAngleVariation = -self->baseAngleVariation;
    }
}

MELShootingStyleDefinition * _Nonnull MELCircularShootingStyleCast(MELCircularShootingStyleDefinition * _Nonnull self) {
    return &self->super;
}

const MELShootingStyleClass MELCircularShootingStyleClass = {
    .update = &MELShootingStyleUpdate,
    .shoot = (void (*)(MELShootingStyle *, MELPoint, GLfloat, MELSpriteType, unsigned int)) &MELCircularShootingStyleShoot,
    .invert = (void (*)(MELShootingStyle *)) &MELCircularShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
