//
//  palette.h
//  melice
//
//  Created by Raphaël Calabro on 30/10/2021.
//

#ifndef palette_h
#define palette_h

#include "melstd.h"

#include "color.h"
#include "size.h"
#include "point.h"
#include "decorator.h"
#include "list.h"
#include "textureatlas.h"

typedef struct melpalette MELPalette;
typedef struct melmap MELMap;

typedef struct {
    /**
     * Allocates a byte array and paints the tile at the given index with palette's preferred tile size.
     *
     * Returned byte count can be obtained by calling: <code>MELPaletteByteCount(palette.tileSize, MELIntSizeMake(1, 1));</code>.
     *
     * Each color is composed of 4 components: RGBA. Each component is 8 bits long and each color is 4 bytes long.
     *
     * @param self Palette instance.
     * @param tileIndex Index of the tile to draw.
     * @returns An array of bytes representing the tile at the given index. You are responsible for freeing the returned array.
     */
    uint8_t * _Nullable (* _Nonnull paintTile)(MELPalette * _Nonnull self, int tileIndex);

    /**
     * Allocates a byte array and paints the given area of the given map with palette's preferred tile size.
     *
     * Returned byte count can be obtained by calling: <code>MELPaletteByteCount(palette.tileSize, areaToRender);</code>.
     *
     * Each color is composed of 4 components: RGBA. Each component is 8 bits long and each color is 4 bytes long.
     *
     * @param self Palette instance.
     * @param map Map to draw.
     * @param areaToRender Size of the area to render.
     * @returns An array of bytes representing the area to render of the given map. You are responsible for freeing the returned array.
     */
    uint8_t * _Nullable (* _Nonnull paintMap)(MELPalette * _Nonnull self, MELMap map, MELIntSize areaToRender);
} MELPaletteClass;

typedef struct melpalette {
    /// Class of this palette.
    MELPaletteClass * _Nonnull class;
    /// Palette name.
    uint16_t * _Nullable name;
    /// Preferred tile size or MELSizeZero if none.
    MELIntSize tileSize;
    /// Number of tiles in this palette.
    uint32_t count;
} MELPalette;

MELListDefine(MELPalette);

/**
 * Returns the number of bytes required to paint an image made of the given amount of tiles with the given tile size.
 *
 * @param tileSize Size of each tile.
 * @param imageSize Number of tiles in the image to paint.
 * @returns The number of bytes required to paint an image with the given requirements.
 */
size_t MELPaletteByteCount(MELIntSize tileSize, MELIntSize imageSize);

/**
 * Creates a new texture atlas with the content of this palette.
 *
 * @param self Palette instance.
 * @returns A new texture atlas with the tiles of this palette.
 */
MELTextureAtlas MELPaletteToTextureAtlas(MELPalette * _Nonnull self);

#endif /* palette_h */
