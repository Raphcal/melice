//
//  straightshootingstyle.c
//  shmup
//
//  Created by Raphaël Calabro on 16/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "straightshootingstyle.h"

#include "shotmotion.h"

StraightShootingStyleDefinition * _Nonnull StraightShootingStyleDefinitionAlloc(void) {
    StraightShootingStyleDefinition value = {
        {
            ShootingStyleDefinitionDefaults,
            .shootingStyleAlloc = (ShootingStyleDefinitionShootingStyleAlloc) StraightShootingStyleAlloc
        },
        0,
        MELPointZero
    };
    StraightShootingStyleDefinition *self = malloc(sizeof(StraightShootingStyleDefinition));
    memcpy(self, &value, sizeof(StraightShootingStyleDefinition));
    return self;
}

ShootingStyle * _Nonnull StraightShootingStyleAlloc(const StraightShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    ShootingStyle *self = malloc(sizeof(ShootingStyle));
    *self = ShootingStyleMake(&StraightShootingStyleClass, (const ShootingStyleDefinition *)definition, spriteManager);
    return self;
}

void StraightShootingStyleShoot(ShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer) {
    const StraightShootingStyleDefinition *definition = (const StraightShootingStyleDefinition *) self->definition;
    MELSpriteManager *spriteManager = self->spriteManager;
    
    MELSpriteDefinition spriteDefinition = spriteManager->definitions.memory[definition->super.spriteDefinition];
    spriteDefinition.type = type;
    
    const GLfloat shotSpeed = definition->super.shotSpeed;
    const int damage = definition->super.damage;
    const GLfloat space = definition->space;
    
    const int shotAmount = self->shotAmount;
    
    origin = MELPointAdd(origin, definition->translation);
    
    GLfloat left = origin.x - (((GLfloat)shotAmount - 1) * space) / 2;
    
    for (int index = 0; index < shotAmount; index++) {
        MELPoint speed = MELPointMake(cosf(angle) * shotSpeed, sinf(angle) * shotSpeed);
        MELSprite *shot = MELSpriteAlloc(spriteManager, spriteDefinition, layer);
        
        MELSpriteSetFrameOrigin(shot, MELPointMake(left, origin.y));
        
        MELSpriteSetMotion(shot, ShotMotionAlloc(angle, speed, damage));
        
        left += space;
    }
}

const ShootingStyleClass StraightShootingStyleClass = {
    .update = &ShootingStyleUpdate,
    .shoot = &StraightShootingStyleShoot,
    .invert = &NoShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
