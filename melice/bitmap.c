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
#include "outputstream.h"

#define BITMAP_MAGIC "BM"
#define MAGIC_LENGTH 2
#define VENDOR_LENGTH 4
#define BITMAP_HEADER_LENGTH 14
#define BITMAP_V4_HEADER_LENGTH 108
#define BITFIELDS_COMPRESSION 3
#define DPI_1 39.3701
#define DPI_72 2835

typedef struct {
    char magic[2];
    int32_t fileSize;
    char vendor[4];
    int32_t contentPosition;
} MELBitmapHeader;

typedef struct {
    int32_t headerLength;
    MELIntSize imageSize;
    uint16_t planes;
    int16_t bitsPerPixel;
    int32_t compression;
    int32_t pixelsLength;
    int32_t horizontalDpi;
    int32_t verticalDpi;
    int32_t paletteLength;
    int32_t importantColors;
    uint32_t redMask;
    uint32_t blueMask;
    uint32_t greenMask;
    uint32_t alphaMask;
    char winHeader[4];
    uint8_t empty[36];
    int32_t redGamma;
    int32_t greenGamma;
    int32_t blueGamma;
} MELBitmapV4Header;

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

void MELBitmapSave(const char * _Nonnull path, MELUInt32Color * _Nonnull pixels, MELIntSize size) {
    MELBitmapSaveWithPremultiplication(path, pixels, size, false);
}

int premultiplyAlpha(int value, int alpha) {
    return alpha == -1 ? value : (value * alpha) / 255;
}

void MELBitmapSaveWithPremultiplication(const char * _Nonnull path, MELUInt32Color * _Nonnull pixels, MELIntSize size, MELBoolean applyPremultiplication) {
    MELOutputStream outputStream = MELOutputStreamOpen(path);
    // Magic
    MELOutputStreamWriteByte(&outputStream, 'B');
    MELOutputStreamWriteByte(&outputStream, 'M');
    // File size
    MELOutputStreamWriteInt(&outputStream, BITMAP_HEADER_LENGTH + BITMAP_V4_HEADER_LENGTH + size.width * size.height * sizeof(MELUInt32Color));
    // Vendor
    MELOutputStreamWriteZero(&outputStream, 4);
    // Content start position
    MELOutputStreamWriteInt(&outputStream, BITMAP_HEADER_LENGTH + BITMAP_V4_HEADER_LENGTH);

    // DIB header length
    MELOutputStreamWriteInt(&outputStream, BITMAP_V4_HEADER_LENGTH);
    // Image size
    MELOutputStreamWriteInt(&outputStream, size.width);
    MELOutputStreamWriteInt(&outputStream, size.height);
    // Planes
    MELOutputStreamWriteShort(&outputStream, 1);
    // Bits per pixel
    MELOutputStreamWriteShort(&outputStream, 32);
    // Compression
    MELOutputStreamWriteInt(&outputStream, BITFIELDS_COMPRESSION);
    // Pixels byte count.
    MELOutputStreamWriteInt(&outputStream, size.width * size.height * sizeof(MELUInt32Color));
    // Horizontal and vertical DPI
    MELOutputStreamWriteInt(&outputStream, DPI_72);
    MELOutputStreamWriteInt(&outputStream, DPI_72);
    // Color count.
    MELOutputStreamWriteInt(&outputStream, 0);
    // Important colors (0 means all).
    MELOutputStreamWriteInt(&outputStream, 0);
    // Red binary mask
    MELOutputStreamWriteUInt32(&outputStream, 0x000000FF);
    // Green binary mask
    MELOutputStreamWriteUInt32(&outputStream, 0x0000FF00);
    // Blue binary mask
    MELOutputStreamWriteUInt32(&outputStream, 0x00FF0000);
    // Alpha binary mask
    MELOutputStreamWriteUInt32(&outputStream, 0xFF000000);
    // Win
    MELOutputStreamWriteByte(&outputStream, ' ');
    MELOutputStreamWriteByte(&outputStream, 'n');
    MELOutputStreamWriteByte(&outputStream, 'i');
    MELOutputStreamWriteByte(&outputStream, 'W');
    MELOutputStreamWriteZero(&outputStream, 0x24);
    // Red gamma
    MELOutputStreamWriteInt(&outputStream, 0);
    // Green gamma
    MELOutputStreamWriteInt(&outputStream, 0);
    // Blue gamma
    MELOutputStreamWriteInt(&outputStream, 0);

    // Pixels
    MELUInt8Color *colors = (MELUInt8Color *) pixels;
    for (int y = size.height - 1; y >= 0; y--) {
        for (int x = 0; x < size.width; x++) {
            MELUInt8Color color = colors[y * size.width + x];
            if (applyPremultiplication) {
                MELOutputStreamWriteByte(&outputStream, premultiplyAlpha(color.red, color.alpha));
                MELOutputStreamWriteByte(&outputStream, premultiplyAlpha(color.green, color.alpha));
                MELOutputStreamWriteByte(&outputStream, premultiplyAlpha(color.blue, color.alpha));
            } else {
                MELOutputStreamWriteByte(&outputStream, color.red);
                MELOutputStreamWriteByte(&outputStream, color.green);
                MELOutputStreamWriteByte(&outputStream, color.blue);
            }
            MELOutputStreamWriteByte(&outputStream, color.alpha);
        }
    }
    MELOutputStreamClose(&outputStream);
}

MELIntSize findAnyMapTile(MELMap map, MELTextureAtlas atlas) {
    for (int layerIndex = 0; layerIndex < map.layerCount; layerIndex++) {
        const MELLayer layer = map.layers[layerIndex];
        if (layer.tiles == NULL) {
            continue;
        }
        for (int tileIndex = 0; tileIndex < layer.tileCount; tileIndex++) {
            if (layer.tiles[tileIndex] != -1) {
                return atlas.sources[layer.tiles[tileIndex]].size;
            }
        }
    }
    return MELIntSizeZero;
}

uint8_t * _Nullable MELBitmapDrawMap(MELMap map, MELTextureAtlas atlas, MELIntSize tileCount, size_t * _Nullable byteCount, MELIntSize * _Nullable outTileSize) {
    if (map.layerCount <= 0) {
        return NULL;
    }

    // Assuming every tile has the same size.
    MELIntSize tileSize = findAnyMapTile(map, atlas);
    if (outTileSize != NULL) {
        *outTileSize = tileSize;
    }

    MELIntSize imageSize = MELIntSizeMake(tileCount.width * tileSize.width, tileCount.height * tileSize.height);

    const int pixelCount = imageSize.width * imageSize.height;

    MELUInt8Color *image = malloc(sizeof(MELUInt8Color) * pixelCount);
    if (byteCount != NULL) {
        *byteCount = sizeof(MELUInt8Color) * pixelCount;
    }

    MELUInt32Color *texture = MELBitmapLoad(atlas.texture.path, NULL);

    for (int pixel = 0; pixel < pixelCount; pixel++) {
        MELUInt8Color color = MELColorToMELUInt8Color(map.backgroundColor);
        const int x = pixel % imageSize.width;
        const int y = pixel / imageSize.width;
        const int tileX = x / tileSize.width;
        const int tileY = y / tileSize.height;
        const int tileIndex = tileX + tileY * map.size.width;

        const int xInsideTile = x % tileSize.width;
        const int yInsideTile = y % tileSize.height;

        for (int layerIndex = 0; layerIndex < map.layerCount; layerIndex++) {
            const MELLayer layer = map.layers[layerIndex];
            const int tile = layer.tiles[tileIndex];
            if (tile != -1) {
                const MELIntRectangle tileFrame = atlas.sources[tile];
                const int texturePixel = tileFrame.origin.x + xInsideTile + (tileFrame.origin.y + yInsideTile) * atlas.texture.size.width;
                const MELUInt32Color tileColor = texture[texturePixel];
                color = MELUInt8ColorBlendWithColor(color, MELRGBAUInt32ColorToMELUInt8Color(tileColor));
            }
        }
        image[pixel] = color;
    }

    free(texture);

    return (uint8_t *) image;
}

uint8_t * _Nullable MELBitmapDrawTile(MELTextureAtlas atlas, int tile, MELUInt32Color * _Nullable texture, size_t * _Nullable byteCount, MELIntSize * _Nullable outTileSize) {
    if (tile < 0 || tile >= atlas.frameCount) {
        fprintf(stderr, "Tile index out of bounds 0..<%d: %d", atlas.frameCount, tile);
        return NULL;
    }
    const MELBoolean shouldLoadAndFreeTexture = texture == NULL;
    if (shouldLoadAndFreeTexture) {
        texture = MELBitmapLoad(atlas.texture.path, NULL);
        if (texture == NULL) {
            fprintf(stderr, "Unable to load bitmap image at path %s", atlas.texture.path);
            return NULL;
        }
    }
    MELIntRectangle source = atlas.sources[tile];
    if (outTileSize != NULL) {
        *outTileSize = source.size;
    }
    const int pixelCount = source.size.width * source.size.height;

    MELUInt32Color *image = malloc(sizeof(MELUInt32Color) * pixelCount);
    if (byteCount != NULL) {
        *byteCount = sizeof(MELUInt8Color) * pixelCount;
    }

    MELUInt32Color *sourceRow = texture + source.origin.x + source.origin.y * atlas.texture.size.width;
    MELUInt32Color *targetRow = image;
    const size_t rowSize = sizeof(MELUInt32Color) * source.size.width;
    for (int y = 0; y < source.size.height; y++) {
        memcpy(targetRow, sourceRow, rowSize);
        sourceRow += atlas.texture.size.width;
        targetRow += source.size.width;
    }

    if (shouldLoadAndFreeTexture) {
        free(texture);
    }

    return (uint8_t *) image;
}
