//
//  spritedefinition.c
//  shmup
//
//  Created by Raphaël Calabro on 28/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "spritedefinition.h"

#include <assert.h>

MELSpriteDefinition MELSpriteDefinitionMakeWithInputStream(MELInputStream * _Nonnull inputStream) {
    assert(inputStream->file != NULL);

    MELSpriteDefinition self;
    self.name = MELInputStreamReadNullableString(inputStream);
    /* width */ MELInputStreamReadInt(inputStream);
    /* height */ MELInputStreamReadInt(inputStream);
    self.type = MELInputStreamReadInt(inputStream);
    self.motionName = MELInputStreamReadNullableString(inputStream);
    
    const int32_t animationCount = MELInputStreamReadInt(inputStream);
	self.animationCount = (unsigned int)animationCount;
    MELAnimationDefinition * _Nonnull animations = calloc(animationCount, sizeof(MELAnimationDefinition));
    for (int32_t index = 0; index < animationCount; index++) {
        animations[index] = MELAnimationDefinitionMakeWithInputStream(inputStream);
    }
    self.animations = animations;
    
    return self;
}

MELList(MELSpriteDefinition) MELSpriteDefinitionListMakeWithInputStream(MELInputStream * _Nonnull inputStream) {
    assert(inputStream->file != NULL);
    
    const int32_t definitionCount = MELInputStreamReadInt(inputStream);
    
    MELList(MELSpriteDefinition) definitions = MELListMakeWithInitialCapacity(MELSpriteDefinition, definitionCount);

    for (int32_t index = 0; index < definitionCount; index++) {
        MELListPush(definitions, MELSpriteDefinitionMakeWithInputStream(inputStream));
    }
    
    return definitions;
}

void MELSpriteDefinitionDeinit(MELSpriteDefinition *_Nonnull self) {
	free(self->name);
	self->name = NULL;
	self->type = 0;
	MELAnimationDefinition *animations = self->animations;
	for (unsigned int index = 0; index < self->animationCount; index++) {
		MELAnimationDefinitionDeinit(animations + index);
	}
	free(animations);
	self->animations = NULL;
	free(self->motionName);
	self->motionName = NULL;
}
