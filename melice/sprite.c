//
//  sprite.c
//  shmup
//
//  Created by Raphaël Calabro on 25/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "sprite.h"

#include "spritemanager.h"
#include "animation.h"
#include "nomotion.h"
#include "noanimation.h"
#include "simplespritehitbox.h"
#include "spritehitbox.h"
#include "statichitbox.h"

MELListImplement(MELSpriteRef);

MELSprite * _Nonnull MELSpriteAlloc(MELSpriteManager * _Nonnull manager, MELSpriteDefinition definition, unsigned int layer) {
    MELSprite *self = malloc(sizeof(MELSprite));

    MELAnimation *animation = MELAnimationAlloc(definition.animations.memory);
    animation->class->start(animation);

    MELRectangle frame = MELRectangleZero;
    MELIntSize size = manager->atlas.sources[animation->frame.atlasIndex].size;
    frame.size = MELSizeMake((GLfloat)size.width, (GLfloat)size.height);
    
    MELHitbox *hitbox = !MELIntRectangleEquals(animation->frame.hitbox, MELIntRectangleZero) ? MELSpriteHitboxAlloc(self) : MELSimpleSpriteHitboxAlloc(self);
    
    *self = (MELSprite) {
        /* parent */ manager,
        /* definition */ definition,
        /* frame */ frame,
        /* direction */ MELDirectionRight,
        /* layer */ layer,
        /* surface */ MELSurfaceArrayAvailableSurface(manager->layerSurfaces + layer),
        /* isRemoved */ false,
        /* hitbox */ hitbox,
        /* motion */ MELNoMotionAlloc(),
        /* animationIndex */ 0,
        /* animation */ animation
    };

	MELListPush(manager->sprites, self);
    
    int *groupForType = manager->groupForType;
    if (groupForType != NULL) {
        const int group = groupForType[definition.type];
        MELListPush(manager->groups[group], self);
    }

    return self;
}

MELSprite * _Nonnull MELSpriteAllocStandalone(MELSpriteDefinition definition) {
    MELSprite *self = calloc(1, sizeof(MELSprite));
    self->definition = definition;
    self->direction = MELDirectionRight;
    self->motion = MELNoMotionAlloc();
    self->animation = MELNoAnimationAlloc(NULL);
    self->hitbox = MELSimpleSpriteHitboxAlloc(self);
    return self;
}

void MELSpriteRefDeinit(MELSpriteRef *_Nonnull self) {
    MELSpriteDeinit(*self);
    free(*self);
    *self = NULL;
}

void MELSpriteDeinit(MELSprite *_Nonnull self) {
    MELSpriteSetMotion(self, NULL);
    MELSpriteSetHitbox(self, NULL);
    MELSpriteSetAnimation(self, NULL);
    
    MELSurfaceArrayReleaseSurface(self->surface.parent, self->surface);
    self->parent = NULL;
}

void MELSpriteSetFrame(MELSprite * _Nonnull self, MELRectangle frame) {
    self->frame = frame;
    MELSurfaceSetVerticesWithRectangle(self->surface, MELRectangleMake(frame.origin.x - frame.size.width / 2, frame.origin.y - frame.size.height / 2, frame.size.width, frame.size.height));
}

void MELSpriteSetFrameOrigin(MELSprite * _Nonnull self, MELPoint origin) {
    MELRectangle frame = self->frame;
    frame.origin = origin;
    MELSpriteSetFrame(self, frame);
}

void MELSpriteSetMotion(MELSprite * _Nonnull self, MELMotion * _Nullable motion) {
    MELMotion *oldMotion = self->motion;
    if (oldMotion != NULL) {
        oldMotion->class->unload(oldMotion, self);
        free(oldMotion);
    }
    self->motion = motion;
}

void MELSpriteSetHitbox(MELSprite * _Nonnull self, MELHitbox * _Nullable hitbox) {
    MELHitbox *oldHitbox = self->hitbox;
    MELHitboxDeinit(oldHitbox);
    free(oldHitbox);
    self->hitbox = hitbox;
}

void MELSpriteSetAnimation(MELSprite * _Nonnull self, MELAnimation * _Nullable animation) {
    // No call to deinit, animations can be freed directly.
    free(self->animation);
    self->animation = animation;
    if (animation != NULL) {
        animation->class->start(animation);
    }
}

void MELSpriteUpdate(MELSprite * _Nonnull self, MELTimeInterval timeSinceLastUpdate) {
    MELMotion *motion = self->motion;
    motion->class->update(motion, self, timeSinceLastUpdate);
    MELAnimation *animation = self->animation;
    const MELAnimationClass *animationClass = animation->class;
    animationClass->update(animation, timeSinceLastUpdate);
    animationClass->draw(animation, self);
}

void MELSpriteRemove(MELSprite * _Nonnull self) {
    self->isRemoved = true;
    MELSpriteSetHitbox(self, MELStaticHitboxAllocate(MELRectangleZero));

    MELSpriteManager *spriteManager = self->parent;
    
    MELListRemoveElement(spriteManager->sprites, self);
    MELListPush(spriteManager->removalPending, self);
    
    const int *groupForType = spriteManager->groupForType;
    const int group = groupForType != NULL ? groupForType[self->definition.type] : -1;
    if (group >= 0 && group < spriteManager->groupCount) {
        MELListRemoveElement(spriteManager->groups[group], self);
    }
}
