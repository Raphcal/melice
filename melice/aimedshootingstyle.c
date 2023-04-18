//
//  aimedshootingstyle.c
//  shmup
//
//  Created by Raphaël Calabro on 15/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "aimedshootingstyle.h"

#include "shotmotion.h"
#include "random.h"

MELAimedShootingStyleDefinition * _Nonnull MELShootingStyleDefinitionAlloc(void) {
    MELAimedShootingStyleDefinition value = (MELAimedShootingStyleDefinition) {
        {
            MELShootingStyleDefinitionDefaults,
            .shootingStyleAlloc = (MELShootingStyleDefinitionShootingStyleAlloc) MELAimedShootingStyleAlloc
        },
        MELSpriteTypePlayer
    };
    MELAimedShootingStyleDefinition *self = malloc(sizeof(MELAimedShootingStyleDefinition));
    memcpy(self, &value, sizeof(MELAimedShootingStyleDefinition));
    return self;
}

MELShootingStyle * _Nonnull MELAimedShootingStyleAlloc(const MELAimedShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    MELShootingStyle *self = malloc(sizeof(MELShootingStyle));
    *self = MELShootingStyleMake(&MELAimedShootingStyleClass, (const MELShootingStyleDefinition *)definition, spriteManager);
    return self;
}

MELList(MELSpriteRef) MELAimedShootingStyleGetTargets(MELShootingStyle * _Nonnull self) {
    const MELAimedShootingStyleDefinition *definition = (const MELAimedShootingStyleDefinition *) self->definition;
    MELSpriteManager *spriteManager = self->spriteManager;
    // TODO: Filtrer le groupe pour ne prendre que les sprites dont la type est TargetType car plusieurs types peuvent être dans le même groupe.
    return spriteManager->groups[spriteManager->groupForType[definition->targetType]];
}

void MELAimedShootingStyleShoot(MELShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer) {
    const MELAimedShootingStyleDefinition *definition = (const MELAimedShootingStyleDefinition *) self->definition;
    MELSpriteManager *spriteManager = self->spriteManager;

    MELSpriteDefinition spriteDefinition = spriteManager->definitions.memory[definition->super.spriteDefinition];
    spriteDefinition.type = type;
    
    MELList(MELSpriteRef) targets = MELAimedShootingStyleGetTargets(self);
    
    const GLfloat shotSpeed = definition->super.shotSpeed;
    const int damage = definition->super.damage;
    
    const int shotAmount = self->shotAmount;
    for (int index = 0; index < shotAmount; index++) {
        GLfloat angleToTarget;
        if (targets.count > 0) {
            MELSpriteRef target = targets.memory[MELRandomInt((int)targets.count)];
            angleToTarget = MELPointAngleToPoint(target->frame.origin, origin);
        } else {
            angleToTarget = angle;
        }
        
        MELPoint speed = MELPointMake(cosf(angleToTarget) * shotSpeed, sinf(angleToTarget) * shotSpeed);
        MELSprite *shot = MELSpriteAlloc(spriteManager, spriteDefinition, layer);
        
        MELSpriteSetFrameOrigin(shot, origin);

        MELSpriteSetMotion(shot, MELShotMotionAlloc(angleToTarget, speed, damage));
    }
}

const MELShootingStyleClass MELAimedShootingStyleClass = {
    .update = &MELShootingStyleUpdate,
    .shoot = &MELAimedShootingStyleShoot,
    .invert = &NoShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
