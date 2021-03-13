//
//  texture.c
//  melice
//
//  Created by Raphaël Calabro on 13/03/2021.
//

#include "texture.h"

#include <assert.h>
#include <string.h>
#include "../io/inputstream.h"

#define MAGIC_LENGTH 2
#define VENDOR_LENGTH 4
#define BITMAP_HEADER_LENGTH 14
#define BITMAP_V4_HEADER_LENGTH 108
#define BITFIELDS_COMPRESSION 3

uint32_t * _Nullable MELLoadBMP(char * _Nonnull path, MELIntSize * _Nonnull size);

MELTexture MELTextureMake(char * _Nonnull path) {
    MELIntSize size;
    void * _Nullable pixels = MELLoadBMP(path, &size);
    
    if (pixels == NULL) {
        printf("MELLoadBMP failed");
        return (MELTexture) { MELIntSizeZero, 0 };
    }
    
    MELTexture self;
    self.size = size;
    
    glGenTextures(1 , &self.name);
    glBindTexture(GL_TEXTURE_2D, self.name);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    free(pixels);
    
    return self;
}

void MELTextureDeinit(MELTexture * _Nonnull self) {
    self->size = MELIntSizeZero;
    glDeleteTextures(1, &self->name);
    self->name = 0;
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

uint32_t * _Nullable MELLoadBMP(char * _Nonnull path, MELIntSize * _Nonnull size) {
    MELInputStream inputStream = MELInputStreamOpen(path);
    char header[2];
    MELInputStreamRead(&inputStream, header, MAGIC_LENGTH);

    if (strncmp(header, "BM", MAGIC_LENGTH) != 0) {
        // Bad header.
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
    if (strncmp(header, " niW", MAGIC_LENGTH) != 0) {
        // Bad header.
        return NULL;
    }

    MELInputStreamSkipBytes(&inputStream, 0x24);

    // Red gamma
    MELInputStreamSkipBytes(&inputStream, sizeof(int32_t));
    // Green gamma
    MELInputStreamSkipBytes(&inputStream, sizeof(int32_t));
    // Blue gamma
    MELInputStreamSkipBytes(&inputStream, sizeof(int32_t));

    // Pixels
    uint32_t *pixels = malloc(pixelsLength);
    MELInputStreamRead(&inputStream, pixels, pixelsLength);

    return pixels;
}
