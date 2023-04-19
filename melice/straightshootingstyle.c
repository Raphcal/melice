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
    
    MELSpriteDefinition bulletDefinition = spriteManager->definitions.memory[definition->super.bulletDefinition];
    bulletDefinition.type = type;
    
    const GLfloat bulletSpeed = definition->super.bulletSpeed;
    const int damage = definition->super.damage;
    const GLfloat space = definition->space;
    
    const int bulletAmount = self->bulletAmount;
    
    origin = MELPointAdd(origin, definition->translation);
    
    GLfloat left = origin.x - (((GLfloat)bulletAmount - 1) * space) / 2;
    
    for (int index = 0; index < bulletAmount; index++) {
        MELPoint speed = MELPointMake(cosf(angle) * bulletSpeed, sinf(angle) * bulletSpeed);
        MELSprite *shot = MELSpriteAlloc(spriteManager, bulletDefinition, layer);
        
        MELSpriteSetFrameOrigin(shot, MELPointMake(left, origin.y));
        
        MELSpriteSetMotion(shot, MELBulletMotionAlloc(angle, speed, damage));
        
        left += space;
    }
}

MELShootingStyleDefinition * _Nonnull MELStraightShootingStyleCast(MELStraightShootingStyleDefinition * _Nonnull self) {
    return &self->super;
}

const MELShootingStyleClass MELStraightShootingStyleClass = {
    .update = &MELShootingStyleUpdate,
    .shoot = &MELStraightShootingStyleShoot,
    .invert = &NoShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
