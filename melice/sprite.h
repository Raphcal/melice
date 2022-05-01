//
//  sprite.h
//  shmup
//
//  Created by Raphaël Calabro on 25/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef sprite_h
#define sprite_h

#include "melstd.h"

#include "direction.h"
#include "hitbox.h"
#include "spritedefinition.h"
#include "rectangle.h"
#include "surface.h"

typedef struct melanimation MELAnimation;
typedef struct melmotion MELMotion;
typedef struct melspritemanager MELSpriteManager;

/**
 * A sprite.
 */
typedef struct melsprite {
    MELSpriteManager * _Nullable parent;
    MELSpriteDefinition definition;
    int type;
    
    MELRectangle frame;
    MELDirection direction;
    
    unsigned int layer;
    MELSurface surface;
    
    MELBoolean isRemoved;
    
    MELHitbox * _Nullable hitbox;
    MELMotion * _Nullable motion;
    
	unsigned int animationIndex;
    MELAnimation * _Nonnull animation;
} MELSprite;

/**
 * Pointer to a sprite.
 */
typedef MELSprite * _Nullable MELSpriteRef;

MELListDefine(MELSpriteRef);

void MELSpriteDeinit(MELSprite * _Nonnull self);

void MELSpriteRefDeinit(MELSpriteRef *_Nonnull self);

MELSprite * _Nonnull MELSpriteAlloc(MELSpriteManager * _Nonnull manager, MELSpriteDefinition definition, unsigned int layer);

MELSprite * _Nonnull MELSpriteAllocStandalone(MELSpriteDefinition definition);

void MELSpriteSetFrame(MELSprite * _Nonnull self, MELRectangle frame);

void MELSpriteSetFrameOrigin(MELSprite * _Nonnull self, MELPoint origin);

void MELSpriteSetMotion(MELSprite * _Nonnull self, MELMotion * _Nullable motion);

void MELSpriteSetHitbox(MELSprite * _Nonnull self, MELHitbox * _Nullable hitbox);

void MELSpriteSetAnimation(MELSprite * _Nonnull self, MELAnimation * _Nullable animation);

void MELSpriteUpdate(MELSprite * _Nonnull self, MELTimeInterval timeSinceLastUpdate);

void MELSpriteRemove(MELSprite * _Nonnull self);

/*
 
 public func isLookingToward(_ point: Point<GLfloat>) -> Bool
 
 public func destroy()
 
 public func explode(_ definition: Int)
 
 public func setAnimation(_ animationName: AnimationName, force: Bool = default)
 
 public func setAnimation(_ animationName: AnimationName, onEnd: @escaping () -> Void)
 
 public func setBlinkingWith(duration: TimeInterval)
 
 public func setBlinkingWith(rate: TimeInterval)
 
 public func setBlinking(_ blinking: Bool)
 
 */

#endif /* sprite_h */
