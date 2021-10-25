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

MELTexture MELTextureMake(const char * _Nonnull path) {
    MELTexture self = {NULL, {0, 0}, 0};
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

void MELTextureLoad(MELTexture * _Nonnull self) {
    assert(self->path != NULL);

    if (self->name != 0) {
        printf("MELTextureLoad: texture seems to be already loaded. Was bound with name %ud\n", self->name);
        return;
    }

    MELIntSize size;
    void * _Nullable pixels = MELBitmapLoad(self->path, &size);
    
    if (pixels == NULL) {
        printf("MELLoadBMP failed\n");
        return;
    }
    self->size = size;

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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
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
