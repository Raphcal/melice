//
//  animationdefinitionframes.h
//  melice
//
//  Created by Raphaël Calabro on 22/07/2022.
//

#ifndef animationdefinitionframes_h
#define animationdefinitionframes_h

#include "melstd.h"

#include "animationframe.h"
#include "imagepaletteimage.h"

typedef struct {
    /**
     * Number of frames in this animation.
     */
    unsigned int frameCount;
    /**
     * Frames.
     */
    MELAnimationFrame * _Nullable frames;
    /**
     * Frames as editable images.
     */
    MELImagePaletteImage * _Nullable images;
} MELAnimationDefinitionFrames;

MELAnimationDefinitionFrames MELAnimationDefinitionFramesMakeWithAnimationDefinitionFrames(MELAnimationDefinitionFrames other);

#endif /* animationdefinitionframes_h */
