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

AimedShootingStyleDefinition * _Nonnull ShootingStyleDefinitionAlloc(void) {
    AimedShootingStyleDefinition value = (AimedShootingStyleDefinition) {
        {
            ShootingStyleDefinitionDefaults,
            .shootingStyleAlloc = (ShootingStyleDefinitionShootingStyleAlloc) AimedShootingStyleAlloc
        },
        MELSpriteTypePlayer
    };
    AimedShootingStyleDefinition *self = malloc(sizeof(AimedShootingStyleDefinition));
    memcpy(self, &value, sizeof(AimedShootingStyleDefinition));
    return self;
}

ShootingStyle * _Nonnull AimedShootingStyleAlloc(const AimedShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    ShootingStyle *self = malloc(sizeof(ShootingStyle));
    *self = ShootingStyleMake(&AimedShootingStyleClass, (const ShootingStyleDefinition *)definition, spriteManager);
    return self;
}

MELList(MELSpriteRef) AimedShootingStyleGetTargets(ShootingStyle * _Nonnull self) {
    const AimedShootingStyleDefinition *definition = (const AimedShootingStyleDefinition *) self->definition;
    MELSpriteManager *spriteManager = self->spriteManager;
    // TODO: Filtrer le groupe pour ne prendre que les sprites dont la type est TargetType car plusieurs types peuvent être dans le même groupe.
    return spriteManager->groups[spriteManager->groupForType[definition->targetType]];
}

void AimedShootingStyleShoot(ShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer) {
    const AimedShootingStyleDefinition *definition = (const AimedShootingStyleDefinition *) self->definition;
    MELSpriteManager *spriteManager = self->spriteManager;

    MELSpriteDefinition spriteDefinition = spriteManager->definitions.memory[definition->super.spriteDefinition];
    spriteDefinition.type = type;
    
    MELList(MELSpriteRef) targets = AimedShootingStyleGetTargets(self);
    
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

        MELSpriteSetMotion(shot, ShotMotionAlloc(angleToTarget, speed, damage));
    }
}

const ShootingStyleClass AimedShootingStyleClass = {
    .update = &ShootingStyleUpdate,
    .shoot = &AimedShootingStyleShoot,
    .invert = &NoShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
