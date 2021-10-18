//
//  textureatlas.c
//  shmup
//
//  Created by Raphaël Calabro on 03/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "textureatlas.h"

#include <assert.h>
#include <string.h>
#include "filemanager.h"
#include "animation.h"

#define ATLAS_EXTENSION ".atlas"
#define ATLAS_EXTENSION_LENGTH 6

#define TEXTURE_EXTENSION ".bmp"
#define TEXTURE_EXTENSION_LENGTH 4

MELTextureAtlas MELTextureAtlasMakeWithPath(const char * _Nonnull path) {
    MELFileManager *fileManager = MELFileManagerGetSharedInstance();
    size_t pathLength = strlen(path);
    
    char *textureName = calloc(pathLength + TEXTURE_EXTENSION_LENGTH + 1, sizeof(char));
    strcpy(textureName, path);
    strcat(textureName, TEXTURE_EXTENSION);
    
    char *texturePath = MELFileManagerPathForAsset(fileManager, textureName);
    free(textureName);
    MELTexture texture = MELTextureMake(texturePath);
    free(texturePath);
    
    char *atlasName = calloc(pathLength + ATLAS_EXTENSION_LENGTH + 1, sizeof(char));
    strcpy(atlasName, path);
    strcat(atlasName, ATLAS_EXTENSION);
    
    MELInputStream inputStream = MELFileManagerOpenAsset(fileManager, atlasName);
    free(atlasName);
    if (inputStream.file) {
        MELTextureAtlas self = MELTextureAtlasMakeWithInputStreamAndTexture(&inputStream, texture);
        MELInputStreamClose(&inputStream);
        return self;
    }
    return (MELTextureAtlas) {
        {
            NULL,
            MELIntSizeZero,
            0
        },
        0,
        NULL
    };
}

MELTextureAtlas MELTextureAtlasMakeWithInputStreamAndTexture(MELInputStream * _Nonnull inputStream, MELTexture texture) {
    assert(texture.size.width != 0 && texture.size.height != 0);

    MELTextureAtlas self;
    self.texture = texture;
    self.frameCount = MELInputStreamReadInt(inputStream);
    
    size_t size = sizeof(MELIntRectangle) * self.frameCount;
    MELIntRectangle *sources = malloc(size);
    MELInputStreamRead(inputStream, sources, size);
    
    self.sources = sources;
    
    MELIntSize textureSize = texture.size;
    self.frames = malloc(sizeof(MELRectangle) * self.frameCount);
    for (int index = 0; index < self.frameCount; index++) {
        const MELIntRectangle source = sources[index];
        self.frames[index] = MELRectangleMake((GLfloat)source.origin.x / textureSize.width, (GLfloat)source.origin.y / textureSize.height, (GLfloat)source.size.width / textureSize.width, (GLfloat)source.size.height / textureSize.height);
    }
    
    return self;
}

void MELTextureAtlasDeinit(MELTextureAtlas * _Nonnull self) {
    MELTextureDeinit(&self->texture);
    self->frameCount = 0;
    free(self->frames);
    self->frames = NULL;
    free(self->sources);
    self->sources = NULL;
}

MELRectangle MELTextureAtlasGetAnimationFrameRectangle(MELTextureAtlas self, MELAnimation * _Nonnull animation) {
    const int index = animation->frame.atlasIndex;
    assert(index >= 0 && index < self.frameCount);
    return self.frames[index];
}
