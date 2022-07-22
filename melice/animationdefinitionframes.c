//
//  animationdefinitionframes.c
//  melice
//
//  Created by Raphaël Calabro on 22/07/2022.
//

#include "animationdefinitionframes.h"

#include "primitives.h"

MELAnimationDefinitionFrames MELAnimationDefinitionFramesMakeWithAnimationDefinitionFrames(MELAnimationDefinitionFrames other) {
    MELAnimationDefinitionFrames self;
    self.frameCount = other.frameCount;
    if (other.frameCount > 0) {
        self.frames = MELArrayCopy(other.frames, sizeof(MELAnimationFrame) * other.frameCount);
        if (other.images) {
            self.images = malloc(sizeof(MELImagePaletteImage) * other.frameCount);
            for (unsigned int index = 0; index < other.frameCount; index++) {
                MELImagePaletteImage image = other.images[index];
                self.images[index] = MELImagePaletteImageMakeWithImagePaletteImage(image, image.size.width * image.size.height);
            }
        }
    }
    return self;
}
