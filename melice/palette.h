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
#include "map.h"
#include "textureatlas.h"
#include "imagepaletteimage.h"

typedef struct melpalette MELPalette;
typedef MELPalette * _Nullable MELPaletteRef;

typedef struct {
    MELPaletteRef (* _Nonnull initWithPaletteRef)(MELPalette * _Nonnull other);
    /**
     * Deinit the given palette and free its resources.
     * The palette itself is not freed.
     *
     * @param self Instance to deinitialize.
     */
    void (* _Nonnull deinit)(MELPalette * _Nonnull self);
    /**
     * Allocates a byte array and paints the tile at the given index with palette's preferred tile size.
     *
     * Returned byte count can be obtained by calling: <code>MELPaletteByteCount(palette.tileSize, MELIntSizeMake(1, 1));</code>.
     *
     * Each color is composed of 4 components: RGBA. Each component is 8 bits long and each color is 4 bytes long.
     *
     * @param self Palette instauinnce.
     * @param tileIndex Index of the tile to draw.
     * @returns An array of bytes representing the tile at the given index. You are responsible for freeing the returned array.
     */
    uint8_t * _Nullable (* _Nonnull paintTile)(MELPalette * _Nonnull self, unsigned int tileIndex);

    /**
     * Paint the given tile to the given buffer.
     */
    void (* _Nonnull paintTileToBuffer)(MELPalette * _Nonnull self, unsigned int tileIndex, MELIntPoint topLeft, MELUInt32Color * _Nonnull buffer, MELIntSize bufferSize);

    /**
     * Allocates a byte array and paints the given image using this palette.
     */
    uint8_t * _Nullable (* _Nullable paintImage)(MELPalette * _Nonnull self, MELImagePaletteImage image);

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

    void * _Nonnull (* _Nonnull tileAtIndex)(MELPalette * _Nonnull self,  unsigned int tileIndex);
} MELPaletteClass;

typedef struct melpalette {
    /// Class of this palette.
    const MELPaletteClass * _Nonnull class;
    /// Palette name.
    char * _Nullable name;
    /// Preferred tile size or MELSizeZero if none.
    MELIntSize tileSize;
    /// Preferred number of columns in the editor.
    uint8_t columns;
    /// Number of tiles in this palette.
    uint32_t count;
} MELPalette;

MELListDefine(MELPaletteRef);

void MELPaletteDeinit(MELPalette * _Nonnull self);

MELPaletteRef MELPaletteRefMakeWithPaletteRef(MELPaletteRef other);
void MELPaletteRefDeinit(MELPaletteRef * _Nonnull self);

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
