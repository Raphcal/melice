//
//  circularshootingstyle.c
//  shmup
//
//  Created by Raphaël Calabro on 18/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "circularshootingstyle.h"

#include "shotmotion.h"

CircularShootingStyleDefinition * _Nonnull CircularShootingStyleDefinitionAlloc(void) {
    CircularShootingStyleDefinition value = (CircularShootingStyleDefinition) {
        {
            ShootingStyleDefinitionDefaults,
            .shootingStyleAlloc = (ShootingStyleDefinitionShootingStyleAlloc) CircularShootingStyleAlloc
        },
        0,
        0,
        0
    };
    CircularShootingStyleDefinition *self = malloc(sizeof(CircularShootingStyleDefinition));
    memcpy(self, &value, sizeof(CircularShootingStyleDefinition));
    return self;
}

ShootingStyle * _Nonnull CircularShootingStyleAlloc(const CircularShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    CircularShootingStyle *self = malloc(sizeof(CircularShootingStyle));
    *self = (CircularShootingStyle) {
        ShootingStyleMake(&CircularShootingStyleClass, (const ShootingStyleDefinition *)definition, spriteManager),
        definition->baseAngle,
        definition->baseAngleVariation
    };
    return (ShootingStyle *)self;
}

void CircularShootingStyleShoot(CircularShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer) {
    const CircularShootingStyleDefinition *definition = (const CircularShootingStyleDefinition *) self->super.definition;
    MELSpriteManager *spriteManager = self->super.spriteManager;
    
    MELSpriteDefinition spriteDefinition = spriteManager->definitions.memory[definition->super.spriteDefinition];
    spriteDefinition.type = type;
    
    const int shotAmount = self->super.shotAmount;
    
    GLfloat currentAngle = angle + definition->baseAngle;
    const GLfloat angleIncrement = definition->angleIncrement || M_PI * 2 / shotAmount;
    
    const GLfloat shotSpeed = definition->super.shotSpeed;
    const int damage = definition->super.damage;
    
    for (int index = 0; index < shotAmount; index++) {
        MELPoint speed = MELPointMake(cosf(currentAngle) * shotSpeed, sinf(currentAngle) * shotSpeed);
        
        MELSprite *shot = MELSpriteAlloc(spriteManager, spriteDefinition, layer);
        MELSpriteSetFrameOrigin(shot, origin);
        MELSpriteSetMotion(shot, ShotMotionAlloc(currentAngle, speed, damage));
        
        currentAngle += angleIncrement;
    }
    
    self->baseAngle += self->baseAngleVariation;
}

void CircularShootingStyleInvert(CircularShootingStyle * _Nonnull self) {
    if (self->super.definition->inversions & ShootingStyleInversionAngle) {
        self->baseAngleVariation = -self->baseAngleVariation;
    }
}

const ShootingStyleClass CircularShootingStyleClass = {
    .update = &ShootingStyleUpdate,
    .shoot = (void (*)(ShootingStyle *, MELPoint, GLfloat, MELSpriteType, unsigned int)) &CircularShootingStyleShoot,
    .invert = (void (*)(ShootingStyle *)) &CircularShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
