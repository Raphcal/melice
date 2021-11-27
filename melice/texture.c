//
//  texture.c
//  melice
//
//  Created by Raphaël Calabro on 13/03/2021.
//

#include "texture.h"

#include <assert.h>
#include <string.h>
#include "filemanager.h"
#include "bitmap.h"
#include "packmap.h"

const MELTexture MELTextureEmpty = {NULL, NULL, (MELIntSize) {0, 0}, 0};

MELTexture MELTextureMake(const char * _Nonnull path) {
    MELTexture self = MELTextureEmpty;
    const size_t pathLength = strlen(path);
    self.path = calloc(pathLength + 1, sizeof(char));
    memcpy(self.path, path, pathLength);
    self.size = MELBitmapReadSize(path);
    return self;
}

MELTexture MELTextureMakeWithAsset(const char * _Nonnull asset) {
    MELFileManager *fileManager = MELFileManagerGetSharedInstance();
    char *texturePath = MELFileManagerPathForAsset(fileManager, asset);
    MELTexture texture = MELTextureMake(texturePath);
    free(texturePath);
    return texture;
}

MELTexture MELTextureMakeWithPackMap(MELPackMap packMap) {
    MELIntSize size = packMap.size;
    size_t count = size.width * size.height;
    MELUInt32Color *pixels = calloc(count, sizeof(MELUInt32Color));

    MELPackMapElementList elements = packMap.elements;
    for (unsigned int index = 0; index < elements.count; index++) {
        MELPackMapElement element = elements.memory[index];
        MELIntPoint origin = MELIntPointDictionaryGetElementOrigin(packMap.origins, element);
        for (unsigned int y = 0; y < element.size.height; y++) {
            memcpy(pixels + origin.x + (origin.y + y) * size.width, element.pixels + y * element.size.width, element.size.width * sizeof(MELUInt32Color));
        }
    }
    return (MELTexture) {NULL, pixels, size, 0};
}

void MELTextureLoad(MELTexture * _Nonnull self) {
    if (self->name != 0) {
        printf("MELTextureLoad: texture seems to be already loaded. Was bound with name %ud\n", self->name);
        return;
    }

    MELUInt32Color * _Nullable pixels;

    if (self->pixels) {
        pixels = self->pixels;
    } else if (self->path) {
        MELIntSize size;
        pixels = MELBitmapLoad(self->path, &size);
        if (pixels == NULL) {
            fprintf(stderr, "MELTextureLoad: MELLoadBMP failed for texture at path: %s\n", self->path);
            return;
        }
        self->size = size;
    } else {
        fprintf(stderr, "MELTextureLoad: texture path and pixels are NULL\n");
        return;
    }

    GLenum error;

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1 , &self->name);
    error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("glGenTextures: error %d\n", error);
        return;
    }

    glBindTexture(GL_TEXTURE_2D, self->name);
    error = glGetError();
    if (error != GL_NO_ERROR) {
        switch (error) {
            case GL_INVALID_ENUM:
                printf("glBindTexture: Target GL_TEXTURE_2D is not allowable\n"); break;
            case GL_INVALID_VALUE:
                printf("glBindTexture: %d is not a name returned by glGenTextures\n", self->name); break;
            case GL_INVALID_OPERATION:
                printf("glBindTexture: texture %d was previously created with a target that doesn't match that of GL_TEXTURE_2D\n", self->name); break;
            default:
                printf("glBindTexture: other error %d\n", error); break;
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self->size.width, self->size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    free(pixels);

    error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("glTexImage2D: error %d\n", error);
        return;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void MELTextureUnload(MELTexture * _Nonnull self) {
    if (self->name != 0) {
        glDeleteTextures(1, &self->name);
        self->name = 0;
    }
}

void MELTextureDeinit(MELTexture * _Nonnull self) {
    free(self->path);
    self->path = NULL;
    self->size = MELIntSizeZero;
    if (self->name != 0) {
        glDeleteTextures(1, &self->name);
        self->name = 0;
    }
}

void MELTextureBind(MELTexture self) {
    assert(self.name != 0);
    glBindTexture(GL_TEXTURE_2D, self.name);
}

int32_t bitShiftForMask(uint32_t mask) {
    switch (mask) {
        default:
        case 0x000000FF:
            return 0;
        case 0x0000FF00:
            return 8;
        case 0x00FF0000:
            return 16;
        case 0xFF000000:
            return 24;
    }
}
