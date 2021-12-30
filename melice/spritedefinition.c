//
//  spritedefinition.c
//  shmup
//
//  Created by Raphaël Calabro on 28/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "spritedefinition.h"

#include <assert.h>

MELListImplement(MELSpriteDefinition);

MELSpriteDefinition MELSpriteDefinitionMakeWithSpriteDefinition(MELSpriteDefinition other) {
    MELSpriteDefinition self = other;
    self.name = MELStringCopy(other.name);
    self.animations = MELAnimationDefinitionListMakeWithListAndCopyFunction(other.animations, &MELAnimationDefinitionMakeWithAnimationDefinition);
    self.loadScript = MELStringCopy(other.loadScript);
    self.motionName = MELStringCopy(other.motionName);
    return self;
}

MELSpriteDefinition MELSpriteDefinitionMakeWithInputStream(MELInputStream * _Nonnull inputStream) {
    assert(inputStream->file != NULL);

    MELSpriteDefinition self;
    self.name = MELInputStreamReadNullableString(inputStream);
    /* width */ MELInputStreamReadInt(inputStream);
    /* height */ MELInputStreamReadInt(inputStream);
    self.type = MELInputStreamReadInt(inputStream);
    self.motionName = MELInputStreamReadNullableString(inputStream);
    self.loadScript = NULL;
    
    const int32_t animationCount = MELInputStreamReadInt(inputStream);
    self.animations = MELAnimationDefinitionListMakeWithInitialCapacity(animationCount);
    self.animations.count = animationCount;
    for (int32_t index = 0; index < animationCount; index++) {
        self.animations.memory[index] = MELAnimationDefinitionMakeWithInputStream(inputStream);
    }
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
    self->palette = NULL;
    MELAnimationDefinitionListDeinitWithDeinitFunction(&self->animations, &MELAnimationDefinitionDeinit);
	free(self->motionName);
	self->motionName = NULL;
    free(self->loadScript);
    self->loadScript = NULL;
}

MELImagePaletteImage * _Nullable MELSpriteDefinitionFirstNonEmptyImage(MELSpriteDefinition self) {
    MELImagePaletteImage *image = NULL;
    MELAnimationDefinitionList animations = self.animations;
    for (unsigned int index = 0; index < animations.count; index++) {
        MELAnimationDefinition *animationDefinition = animations.memory + index;
        MELImagePaletteImage *animationImage = MELAnimationDefinitionFirstNonEmptyImage(*animationDefinition);
        if (animationImage != NULL) {
            if (!strcmp(animationDefinition->name, "stand")) {
                return animationImage;
            } else if (image == NULL) {
                image = animationImage;
            }
        }
    }
    return image;
}
