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

MELShootingStyle * _Nonnull MELAimedShootingStyleAlloc(const MELShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    MELShootingStyle *self = malloc(sizeof(MELShootingStyle));
    *self = MELShootingStyleMake(&MELAimedShootingStyleClass, definition, spriteManager);
    return self;
}

MELList(MELSpriteRef) MELAimedShootingStyleGetTargets(MELShootingStyle * _Nonnull self) {
    const MELShootingStyleDefinition *definition = self->definition;
    MELSpriteManager *spriteManager = self->spriteManager;
    // TODO: Filtrer le groupe pour ne prendre que les sprites dont la type est TargetType car plusieurs types peuvent être dans le même groupe.
    return spriteManager->groups[spriteManager->groupForType[definition->targetType]];
}

void MELAimedShootingStyleShoot(MELShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer) {
    const MELShootingStyleDefinition *definition = self->definition;
    MELSpriteManager *spriteManager = self->spriteManager;

    MELSpriteDefinition bulletDefinition = spriteManager->definitions.memory[definition->bulletDefinition];
    bulletDefinition.type = type;
    
    MELList(MELSpriteRef) targets = MELAimedShootingStyleGetTargets(self);
    
    const GLfloat bulletSpeed = definition->bulletSpeed;
    const int damage = definition->damage;
    
    const int bulletAmount = self->bulletAmount;
    for (int index = 0; index < bulletAmount; index++) {
        GLfloat angleToTarget;
        if (targets.count > 0) {
            MELSpriteRef target = targets.memory[MELRandomInt((int)targets.count)];
            angleToTarget = MELPointAngleToPoint(target->frame.origin, origin);
        } else {
            angleToTarget = angle;
        }
        
        MELPoint speed = MELPointMake(cosf(angleToTarget) * bulletSpeed, sinf(angleToTarget) * bulletSpeed);
        MELSprite *shot = MELSpriteAlloc(spriteManager, bulletDefinition, layer);
        
        MELSpriteSetFrameOrigin(shot, origin);

        MELSpriteSetMotion(shot, MELBulletMotionAlloc(angleToTarget, speed, damage));
    }
}

const MELShootingStyleClass MELAimedShootingStyleClass = {
    .update = &MELShootingStyleUpdate,
    .shoot = &MELAimedShootingStyleShoot,
    .invert = &NoShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
