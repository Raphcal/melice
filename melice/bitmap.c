//
//  bitmap.c
//  melice
//
//  Created by Raphaël Calabro on 25/10/2021.
//

#include "bitmap.h"

#include <assert.h>
#include <string.h>
#include "inputstream.h"

#define BITMAP_MAGIC "BM"
#define MAGIC_LENGTH 2
#define VENDOR_LENGTH 4
#define BITMAP_HEADER_LENGTH 14
#define BITMAP_V4_HEADER_LENGTH 108
#define BITFIELDS_COMPRESSION 3

MELIntSize MELBitmapReadSize(const char * _Nonnull path) {
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

MELUInt32Color * _Nullable MELBitmapLoad(const char * _Nonnull path, MELIntSize * _Nullable size) {
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
    if (size != NULL) {
        *size = imageSize;
    }

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
    MELUInt32Color *pixels = malloc(pixelsLength);
    MELUInt32Color *line = pixels + (imageSize.height - 1) * imageSize.width;
    const size_t lineLength = imageSize.width * sizeof(MELUInt32Color);
    for (uint32_t y = 0; y < imageSize.height; y++, line -= imageSize.width) {
        MELInputStreamRead(&inputStream, line, lineLength);
    }

    MELInputStreamClose(&inputStream);

    return pixels;
}

MELIntSize findAnyMapTile(MELMap map, MELTextureAtlas atlas) {
    for (int layerIndex = 0; layerIndex < map.layerCount; layerIndex++) {
        const MELLayer layer = map.layers[layerIndex];
        if (layer.tiles == NULL) {
            continue;
        }
        for (int tileIndex = 0; tileIndex < layer.tileCount; tileIndex++) {
            if (layer.tiles[tileIndex] != -1) {
                MELRectangle rect = atlas.frames[layer.tiles[tileIndex]];
                return MELIntSizeMake(rect.size.width * atlas.texture.size.width, rect.size.height * atlas.texture.size.width);
            }
        }
    }
    return MELIntSizeZero;
}

uint8_t * _Nullable MELBitmapDrawMap(MELMap map, MELTextureAtlas atlas, MELIntSize tileCount, size_t * _Nullable pixelCount, MELIntSize * _Nullable outTileSize) {
    if (map.layerCount <= 0) {
        return NULL;
    }

    // Assuming every tile has the same size.
    MELIntSize tileSize = findAnyMapTile(map, atlas);
    if (outTileSize != NULL) {
        *outTileSize = tileSize;
    }

    MELIntSize imageSize = MELIntSizeMake(tileCount.width * tileSize.width, tileCount.height * tileSize.height);

    const int length = imageSize.width * imageSize.height;

    MELUInt8Color *image = malloc(sizeof(MELUInt8Color) * length);
    if (pixelCount != NULL) {
        *pixelCount = sizeof(MELUInt8Color) * length;
    }

    MELUInt32Color *texture = MELBitmapLoad(atlas.texture.path, NULL);

    for (int pixel = 0; pixel < length; pixel++) {
        MELUInt8Color color = MELColorToMELUInt8Color(map.backgroundColor);
        const int x = pixel % imageSize.width;
        const int y = pixel / imageSize.width;
        const int tileX = x / tileSize.width;
        const int tileY = y / tileSize.height;
        const int tileIndex = tileX + tileY * map.size.width;

        const int xInsideTile = x % tileSize.width;
        const int yInsideTile = y % tileSize.height;

        for (int layerIndex = map.layerCount - 1; layerIndex >= 0; layerIndex--) {
            const MELLayer layer = map.layers[layerIndex];
            const int tile = layer.tiles[tileIndex];
            if (tile != -1) {
                const MELRectangle tileFrame = atlas.frames[tile];
                const MELIntPoint location = MELIntPointMake(tileFrame.origin.x * atlas.texture.size.width, tileFrame.origin.y * atlas.texture.size.height);
                const int texturePixel = location.x + xInsideTile + (location.y + yInsideTile) * atlas.texture.size.width;
                const MELUInt32Color tileColor = texture[texturePixel];
                color = MELUInt8ColorBlendWithColor(color, MELRGBAUInt32ColorToMELUInt8Color(tileColor));
                break;
            }
        }
        image[pixel] = color;
    }

    free(texture);

    return (uint8_t *) image;
}

