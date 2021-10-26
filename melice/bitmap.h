//
//  bitmap.h
//  melice
//
//  Created by Raphaël Calabro on 25/10/2021.
//

#ifndef bitmap_h
#define bitmap_h

#include "melstd.h"

#include "size.h"
#include "color.h"
#include "map.h"
#include "textureatlas.h"

MELIntSize MELBitmapReadSize(const char * _Nonnull path);

/**
 * Loads the bitmap file located at the given path.
 *
 * @param path Path to the BMP file.
 * @param size Pointer to a MELIntSize object. Will be set to the bitmap size.
 * @returns Bitmap pixel data.
 */
MELUInt32Color * _Nullable MELBitmapLoad(const char * _Nonnull path, MELIntSize * _Nullable size);

uint8_t * _Nullable MELBitmapDrawMap(MELMap map, MELTextureAtlas atlas, MELIntSize tileCount, size_t * _Nullable byteCount, MELIntSize * _Nullable outTileSize);

uint8_t * _Nullable MELBitmapDrawTile(MELTextureAtlas atlas, int tile, MELUInt32Color * _Nullable texture, size_t * _Nullable byteCount, MELIntSize * _Nullable outTileSize);

#endif /* bitmap_h */
