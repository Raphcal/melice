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
#include "palette.h"
#include "packmap.h"

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
    
    MELTexture texture = MELTextureMakeWithAsset(textureName);
    free(textureName);
    
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
        MELTextureEmpty,
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

MELTextureAtlas MELTextureAtlasMakeWithPackMapAndRefList(MELPackMap packMap, MELRefList content) {
    MELTextureAtlas self;
    self.texture = MELTextureMakeWithPackMap(packMap);
    self.frameCount = (int) content.count;
    self.sources = malloc(sizeof(MELIntRectangle) * content.count);
    self.frames = malloc(sizeof(MELRectangle) * content.count);

    MELIntSize textureSize = self.texture.size;

    for (unsigned int index = 0; index < content.count; index++) {
        MELIntRectangle source;
        if (MELPointerMELIntRectangleTableGet(packMap.origins, (MELPointer) content.memory[index], &source)) {
            self.sources[index] = source;
            self.frames[index] = MELRectangleMake((GLfloat)source.origin.x / textureSize.width, (GLfloat)source.origin.y / textureSize.height, (GLfloat)source.size.width / textureSize.width, (GLfloat)source.size.height / textureSize.height);
        }
    }

    return self;
}

MELTextureAtlas MELTextureAtlasMakeWithPalette(MELPalette * _Nonnull palette) {
    assert(palette != NULL);

    MELPackMapElementList elements = MELPackMapElementListEmpty;
    MELPackMapElementListPushPalette(&elements, palette);
    MELPackMap packMap = MELPackMapMakeWithElements(elements);
    MELPackMapElementListDeinit(&elements);

    const uint32_t count = palette->count;

    MELTextureAtlas self;
    self.texture = MELTextureMakeWithPackMap(packMap);
    self.frameCount = (int) count;
    self.sources = malloc(sizeof(MELIntRectangle) * count);
    self.frames = malloc(sizeof(MELRectangle) * count);

    MELIntSize textureSize = self.texture.size;

    for (unsigned int index = 0; index < count; index++) {
        MELIntRectangle source = MELPackMapFrameForPaletteTile(packMap, palette, index);
        self.sources[index] = source;
        self.frames[index] = MELRectangleMake((GLfloat)source.origin.x / textureSize.width, (GLfloat)source.origin.y / textureSize.height, (GLfloat)source.size.width / textureSize.width, (GLfloat)source.size.height / textureSize.height);
    }

    MELPackMapDeinit(&packMap);

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
