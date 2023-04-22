//
//  particuleshootingstyle.c
//  melice
//
//  Created by Raphaël Calabro on 22/04/2023.
//

#include "particuleshootingstyle.h"

#include "particulemotion.h"
#include "animation.h"
#include "random.h"

MELShootingStyle * _Nonnull MELParticuleShootingStyleAlloc(const MELShootingStyleDefinition * _Nonnull definition, MELSpriteManager * _Nonnull spriteManager) {
    MELShootingStyle *self = malloc(sizeof(MELShootingStyle));
    *self = MELShootingStyleMake(&MELParticuleShootingStyleClass, definition, spriteManager);
    return self;
}

static void MELParticuleShootingStyleShoot(MELShootingStyle * _Nonnull self, MELPoint origin, GLfloat angle, MELSpriteType type, unsigned int layer) {
    const MELShootingStyleDefinition *definition = self->definition;
    MELSpriteManager *spriteManager = self->spriteManager;

    MELSpriteDefinition bulletDefinition = spriteManager->definitions.memory[definition->bulletDefinition];
    bulletDefinition.type = MELSpriteTypeDecor;

    const int animationIndex = definition->animation;
    const float space = definition->space;
    const float halfSpace = space / 2.0f;

    const int bulletAmount = self->bulletAmount;
    for (int index = 0; index < bulletAmount; index++) {
        MELSprite *particule = MELSpriteAlloc(spriteManager, bulletDefinition, layer);
        MELSpriteSetFrameOrigin(particule, (MELPoint) {
            .x = origin.x + MELRandomFloat(space) - halfSpace,
            .y = origin.y + MELRandomFloat(space) - halfSpace,
        });
        MELSpriteSetMotion(particule, MELParticuleMotionAlloc());
        if (animationIndex != 0) {
            MELSpriteSetAnimation(particule, MELAnimationAlloc(bulletDefinition.animations.memory + animationIndex));
        }
    }
}

const MELShootingStyleClass MELParticuleShootingStyleClass = {
    .update = &MELShootingStyleUpdate,
    .shoot = &MELParticuleShootingStyleShoot,
    .invert = &NoShootingStyleInvert,
    .deinit = &NoShootingStyleDeinit,
};
