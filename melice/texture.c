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
#include "inputstream.h"

#define BITMAP_MAGIC "BM"
#define MAGIC_LENGTH 2
#define VENDOR_LENGTH 4
#define BITMAP_HEADER_LENGTH 14
#define BITMAP_V4_HEADER_LENGTH 108
#define BITFIELDS_COMPRESSION 3

MELIntSize MELReadBMPSize(const char * _Nonnull path);
uint32_t * _Nullable MELLoadBMP(const char * _Nonnull path, MELIntSize * _Nonnull size);

MELTexture MELTextureMake(const char * _Nonnull path) {
    MELTexture self = {NULL, {0, 0}, 0};
    const size_t pathLength = strlen(path);
    self.path = calloc(pathLength + 1, sizeof(char));
    memcpy(self.path, path, pathLength);
    self.size = MELReadBMPSize(path);
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
        printf("MELTextureLoad: texture seems to be already loaded. Was bound with name %ud", self->name);
        return;
    }

    MELIntSize size;
    void * _Nullable pixels = MELLoadBMP(self->path, &size);
    
    if (pixels == NULL) {
        printf("MELLoadBMP failed");
        return;
    }
    self->size = size;

    GLenum error;

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1 , &self->name);
    error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("glGenTextures: error %d", error);
        return;
    }

    glBindTexture(GL_TEXTURE_2D, self->name);
    error = glGetError();
    if (error != GL_NO_ERROR) {
        switch (error) {
            case GL_INVALID_ENUM:
                printf("glBindTexture: Target GL_TEXTURE_2D is not allowable"); break;
            case GL_INVALID_VALUE:
                printf("glBindTexture: %d is not a name returned by glGenTextures", self->name); break;
            case GL_INVALID_OPERATION:
                printf("glBindTexture: texture %d was previously created with a target that doesn't match that of GL_TEXTURE_2D", self->name); break;
            default:
                printf("glBindTexture: other error %d", error); break;
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    free(pixels);

    error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("glTexImage2D: error %d", error);
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

MELIntSize MELReadBMPSize(const char * _Nonnull path) {
    MELInputStream inputStream = MELInputStreamOpen(path);
    char header[2];
    MELInputStreamRead(&inputStream, header, MAGIC_LENGTH);

    if (strncmp(header, BITMAP_MAGIC, MAGIC_LENGTH) != 0) {
        // Bad header.
        MELInputStreamClose(&inputStream);
        return MELIntSizeMake(0, 0);
    }

    /* int32_t fileSize = */ MELInputStreamReadInt(&inputStream);
    MELInputStreamSkipBytes(&inputStream, VENDOR_LENGTH);

    /* int32_t contentPosition = */ MELInputStreamReadInt(&inputStream);

    int32_t headerLength = MELInputStreamReadInt(&inputStream);
    assert(headerLength == BITMAP_V4_HEADER_LENGTH);

    MELIntSize imageSize = MELIntSizeMake(MELInputStreamReadInt(&inputStream),
                           MELInputStreamReadInt(&inputStream));
    MELInputStreamClose(&inputStream);
    return imageSize;
}

uint32_t * _Nullable MELLoadBMP(const char * _Nonnull path, MELIntSize * _Nonnull size) {
    MELInputStream inputStream = MELInputStreamOpen(path);
    char header[2];
    MELInputStreamRead(&inputStream, header, MAGIC_LENGTH);

    if (strncmp(header, BITMAP_MAGIC, MAGIC_LENGTH) != 0) {
        // Bad header.
        MELInputStreamClose(&inputStream);
        return NULL;
    }
    /* int32_t fileSize = */ MELInputStreamReadInt(&inputStream);
    MELInputStreamSkipBytes(&inputStream, VENDOR_LENGTH);

    /* int32_t contentPosition = */ MELInputStreamReadInt(&inputStream);

    int32_t headerLength = MELInputStreamReadInt(&inputStream);
    assert(headerLength == BITMAP_V4_HEADER_LENGTH);

    MELIntSize imageSize = MELIntSizeMake(MELInputStreamReadInt(&inputStream),
                           MELInputStreamReadInt(&inputStream));
    *size = imageSize;

    // Planes
    MELInputStreamSkipBytes(&inputStream, sizeof(uint16_t));

    /* int16_t bitsPerPixel = */ MELInputStreamReadShort(&inputStream);

    int32_t compression = MELInputStreamReadInt(&inputStream);
    assert(compression == BITFIELDS_COMPRESSION);

    int32_t pixelsLength = MELInputStreamReadInt(&inputStream);

    // Horizontal DPI
    MELInputStreamSkipBytes(&inputStream, sizeof(int32_t));
    // Vertical DPI
    MELInputStreamSkipBytes(&inputStream, sizeof(int32_t));
    // Palette length
    MELInputStreamSkipBytes(&inputStream, sizeof(int32_t));
    // Important colors
    MELInputStreamSkipBytes(&inputStream, sizeof(int32_t));

    /* uint32_t redMask = */ MELInputStreamReadInt(&inputStream);
    /* uint32_t blueMask = */ MELInputStreamReadInt(&inputStream);
    /* uint32_t greenMask = */ MELInputStreamReadInt(&inputStream);
    /* uint32_t alphaMask = */ MELInputStreamReadInt(&inputStream);

    char winHeader[4];
    MELInputStreamRead(&inputStream, winHeader, 4);
    if (strncmp(winHeader, " niW", MAGIC_LENGTH) != 0) {
        // Bad header.
        MELInputStreamClose(&inputStream);
        return NULL;
    }

    MELInputStreamSkipBytes(&inputStream, 0x24);

    // Red gamma
    MELInputStreamSkipBytes(&inputStream, sizeof(int32_t));
    // Green gamma
    MELInputStreamSkipBytes(&inputStream, sizeof(int32_t));
    // Blue gamma
    MELInputStreamSkipBytes(&inputStream, sizeof(int32_t));

    // Reading pixels from bottom to top.
    uint32_t *pixels = malloc(pixelsLength);
    uint32_t *line = pixels + (imageSize.height - 1) * imageSize.width;
    const size_t lineLength = imageSize.width * sizeof(uint32_t);
    for (uint32_t y = 0; y < imageSize.height; y++, line -= imageSize.width) {
        MELInputStreamRead(&inputStream, line, lineLength);
    }

    MELInputStreamClose(&inputStream);

    return pixels;
}
