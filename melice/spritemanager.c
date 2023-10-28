//
//  spritemanager.c
//  shmup
//
//  Created by Raphaël Calabro on 27/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "spritemanager.h"

#include "motion.h"

const MELSpriteManager MELSpriteManagerEmpty = {};

MELSpriteManager MELSpriteManagerMake(MELList(MELSpriteDefinition) definitions, MELTextureAtlas atlas, MELSurfaceArray *_Nonnull layerSurfaces, unsigned int groupCount, int * _Nullable groupForType) {
	MELSpriteManager self;
	self.definitions = definitions;
	self.atlas = atlas;
	self.layerSurfaces = layerSurfaces;
    self.sprites = MELSpriteRefListEmpty;
	self.removalPending = MELSpriteRefListEmpty;
	self.groupCount = groupCount;
	if (groupCount > 0) {
        self.groups = calloc(groupCount, sizeof(MELSpriteRefList));
	} else {
		self.groups = NULL;
	}
    self.groupForType = groupForType;
	return self;
}

MELSpriteManager *_Nonnull MELSpriteManagerAlloc(MELList(MELSpriteDefinition) definitions, MELTextureAtlas atlas, MELSurfaceArray *_Nonnull layerSurfaces, unsigned int groupCount, int * _Nullable groupForType) {
	MELSpriteManager *self = malloc(sizeof(MELSpriteManager));
	*self = MELSpriteManagerMake(definitions, atlas, layerSurfaces, groupCount, groupForType);
	return self;
}

void MELSpriteManagerDeinit(MELSpriteManager *_Nonnull self) {
	// surfaces and texture atlas should be freed by the parent object.
	MELListDeinitWithDeinitFunction(self->sprites, MELSpriteRefDeinit);
	MELListDeinit(self->removalPending);

	MELList(MELSpriteRef) *groups = self->groups;
	for (unsigned int index = 0; index < self->groupCount; index++) {
		MELListDeinit(groups[index]);
	}
	free(groups);
    
    MELListDeinitWithDeinitFunction(self->definitions, MELSpriteDefinitionDeinit);
    
	self->groups = NULL;
	self->groupCount = 0;
    self->groupForType = NULL;
}

void MELSpriteManagerRemoveAllSprites(MELSpriteManager * _Nonnull self) {
    MELSpriteRefList sprites = self->sprites;
    for (int index = 0; index < sprites.count; index++) {
        MELSprite *sprite = sprites.memory[index];
        MELSpriteDeinit(sprite);
        free(sprite);
    }
    self->sprites.count = 0;
    self->removalPending.count = 0;

    for (unsigned int index = 0; index < self->groupCount; index++) {
        self->groups[index].count = 0;
    }

    MELSurfaceArrayClearPool(self->layerSurfaces);
}

void MELSpriteManagerCommitRemoval(MELSpriteManager * _Nonnull self) {
    MELList(MELSpriteRef) removalPending = self->removalPending;
    for (int index = 0; index < removalPending.count; index++) {
        MELSprite *sprite = removalPending.memory[index];
        MELSpriteDeinit(sprite);
        free(sprite);
    }
    self->removalPending.count = 0;
}

void MELSpriteRefListUpdate(MELList(MELSpriteRef) self, MELTimeInterval timeSinceLastUpdate) {
    MELSpriteRef sprites[self.count];
    memcpy(sprites, self.memory, sizeof(MELSpriteRef) * self.count);
    for (unsigned int index = 0; index < self.count; index++) {
        MELSpriteUpdate(sprites[index], timeSinceLastUpdate);
    }
}

void MELSpriteManagerUpdate(MELSpriteManager *_Nonnull self, MELTimeInterval timeSinceLastUpdate) {
	MELSpriteRefListUpdate(self->sprites, timeSinceLastUpdate);
    MELSpriteManagerCommitRemoval(self);
}
