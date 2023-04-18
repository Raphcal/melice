//
//  straightshootingstyle.c
//  shmup
//
//  Created by Raphaël Calabro on 16/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "straightshootingstyle.h"

#include "shotmotion.h"

MELStraightShootingStyleDefinition * _Nonnull MELStraightShootingStyleDefinitionAlloc(void) {
    MELStraightShootingStyleDefinition value = {
        {
            MELShootingStyleDefinitionDefaults,
            .shootingStyleAlloc = (MELShootingStyleDefinitionShootingStyleAlloc) MELStraightShootingStyleAlloc
        },
        0,
        MELPointZero
    };
    MELStraightShootingStyleDefinition *self = malloc(sizeof(MELStraightShootingStyleDefinition));
    memcpy(self, &value, sizeof(MELStraightShootingStyleDefinition));
    return self;
}

MELShootingStyle * _Nonnull MELStraightShootingStyleAlloc(const MELStraightShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    MELShootingStyle *self = malloc(sizeof(MELShootingStyle));
    *self = MELShootingStyleMake(&MELStraightShootingStyleClass, (const MELShootingStyleDefinition *)definition, spriteManager);
    return self;
}

static void MELStraightShootingStyleShoot(MELShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer) {
    const MELStraightShootingStyleDefinition *definition = (const MELStraightShootingStyleDefinition *) self->definition;
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
        
        MELSpriteSetMotion(shot, MELShotMotionAlloc(angle, speed, damage));
        
        left += space;
    }
}

const MELShootingStyleClass MELStraightShootingStyleClass = {
    .update = &MELShootingStyleUpdate,
    .shoot = &MELStraightShootingStyleShoot,
    .invert = &NoShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
