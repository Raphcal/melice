//
//  animationdefinition.c
//  shmup
//
//  Created by Raphaël Calabro on 28/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "animationdefinition.h"

#include <assert.h>
#include "primitives.h"

MELListImplement(MELAnimationDefinition);

MELAnimationDefinition MELAnimationDefinitionMakeWithAnimationDefinition(MELAnimationDefinition other) {
    MELAnimationDefinition self = other;
    self.name = MELStringCopy(other.name);
    self.frames = MELArrayCopy(other.frames, sizeof(MELAnimationFrame) * other.frameCount);
    if (other.images) {
        self.images = calloc(other.frameCount, sizeof(MELImagePaletteImage));
        for (unsigned int index = 0; index < other.frameCount; index++) {
            MELImagePaletteImage image = other.images[index];
            MELSizeDecorator *size = MELDecoratorRefListGetSizeDecorator(image.decorators);
            if (size) {
                self.images[index] = MELImagePaletteImageMakeWithImagePaletteImage(image, size->size.width * size->size.height);
            }
        }
    }
    return self;
}

MELAnimationDefinition MELAnimationDefinitionMakeWithInputStream(MELInputStream * _Nonnull inputStream) {
    assert(inputStream->file != NULL);

    MELAnimationDefinition self;
    self.name = MELInputStreamReadString(inputStream);
    self.frequency = MELInputStreamReadInt(inputStream);
    const MELBoolean looping = MELInputStreamReadBoolean(inputStream);
    
    // Directions et frames
    const int32_t directionCount = MELInputStreamReadInt(inputStream);
    
    MELAnimationFrame * _Nullable mainFrames = NULL;
    int32_t mainFrameCount = 0;
    
    for (int32_t direction = 0; direction < directionCount; direction++) {
        const double key = MELInputStreamReadDouble(inputStream);
        
        // Frames
        const int32_t frameCount = MELInputStreamReadInt(inputStream);
        MELAnimationFrame * _Nonnull frames = calloc(frameCount, sizeof(MELAnimationFrame));
        
        for (int32_t frame = 0; frame < frameCount; frame++) {
            frames[frame] = MELAnimationFrameMakeWithInputStream(inputStream);
        }
        
        if (key == 0 || mainFrames == NULL) {
            if (mainFrames != NULL) {
                free(mainFrames);
            }
            mainFrames = frames;
            mainFrameCount = frameCount;
        } else {
            free(frames);
        }
    }
    
    self.frameCount = mainFrameCount;
    self.frames = mainFrames;
    self.images = NULL;
    self.type = MELAnimationTypeForFrameCountAndLooping(mainFrameCount, looping);
    self.isScrolling = false;
    return self;
}

MELTimeInterval MELAnimationDefinitionDuration(MELAnimationDefinition self) {
    return (MELTimeInterval)self.frameCount / self.frequency;
}

void MELAnimationDefinitionDeinit(MELAnimationDefinition *_Nonnull self) {
	free(self->name);
	self->name = NULL;
	free(self->frames);
	self->frames = NULL;
    free(self->images);
    self->images = NULL;
	self->frameCount = 0;
	self->frequency = 0;
	self->type = MELAnimationTypeNone;
    self->isScrolling = false;
}

MELImagePaletteImage * _Nullable MELAnimationDefinitionFirstNonEmptyImage(MELAnimationDefinition self) {
    if (self.images == NULL) {
        return NULL;
    }
    for (unsigned int index = 0; index < self.frameCount; index++) {
        MELImagePaletteImage *image = self.images + index;
        if (!MELImagePaletteImageIsEmpty(*image)) {
            return image;
        }
    }
    return NULL;
}

