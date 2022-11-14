//
//  spritemanager.h
//  shmup
//
//  Created by Raphaël Calabro on 27/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef spritemanager_h
#define spritemanager_h

#include "melstd.h"

#include "sprite.h"
#include "spritedefinition.h"
#include "referencepool.h"
#include "textureatlas.h"
#include "surfacearray.h"
#include "list.h"

typedef struct melspritemanager MELSpriteManager;

struct melspritemanager {
	MELList(MELSpriteDefinition) definitions;
    MELTextureAtlas atlas;
    MELSurfaceArray * _Nullable layerSurfaces;
    MELList(MELSpriteRef) sprites;
    MELList(MELSpriteRef) removalPending;
	unsigned int groupCount;
    MELList(MELSpriteRef) * _Nullable groups;
    /**
     * Not deallocated by the deinit method to allow the use of an external variable common to multiple sprite managers.
     */
    int * _Nullable groupForType;
};

extern const MELSpriteManager MELSpriteManagerEmpty;

MELSpriteManager MELSpriteManagerMake(MELList(MELSpriteDefinition) definitions, MELTextureAtlas atlas, MELSurfaceArray * _Nonnull layerSurfaces, unsigned int groupCount, int * _Nullable groupForType);

MELSpriteManager * _Nonnull MELSpriteManagerAlloc(MELList(MELSpriteDefinition) definitions, MELTextureAtlas atlas, MELSurfaceArray * _Nonnull layerSurfaces, unsigned int groupCount, int * _Nullable groupForType);

void MELSpriteManagerDeinit(MELSpriteManager * _Nonnull self);

void MELSpriteManagerRemoveAllSprites(MELSpriteManager * _Nonnull self);

void MELSpriteManagerUpdate(MELSpriteManager * _Nonnull self, MELTimeInterval timeSinceLastUpdate);

/*
 
 public func sprite(_ definition: Int) -> Sprite
 
 public func sprite(_ definition: Int?) -> Sprite?
 
 public func sprite(_ definition: Int, topLeft: Point<GLfloat>) -> Sprite
 
 /// Créé un sprite non animé à partir des informations données.
 ///
 /// - parameter parent: Sprite contenant la définition à utiliser.
 /// - parameter animation: Nom de l'animation à sélectionner.
 /// - parameter frame: Indice de l'étape d'animation à utiliser. La taille du nouveau sprite sera celle de l'étape d'animation.
 /// - returns: Un nouveau sprite.
 public func sprite(_ parent: Sprite, animation: AnimationName, frame: Int) -> Sprite
 
 /// Créé un sprite à partir de la définition donnée et lie le sprite à l'information donnée (si non nil).
 ///
 /// Il s'agit de la méthode principale pour créer un sprite. Elle est appelée par toute les autres méthodes de création.
 ///
 /// - parameter definition: Définition du sprite.
 /// - parameter info: Informations du loader sur le sprite à créer.
 /// - parameter after: Si non nil, la référence du nouveau sprite sera (si possible) supérieur à celle du sprite donné.
 /// - returns: Un nouveau sprite.
 open func sprite(_ definition: SpriteDefinition, info: SpriteInfo? = default, after: Sprite? = default) -> Sprite
 
 public func removeOrphanSprites()
 
 public func commitRemoval()
 
 public func clear()
 
 */

#endif /* spritemanager_h */
