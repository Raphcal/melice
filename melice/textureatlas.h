//
//  textureatlas.h
//  shmup
//
//  Created by Raphaël Calabro on 03/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef textureatlas_h
#define textureatlas_h

#include "melstd.h"
#include "texture.h"
#include "rectangle.h"
#include "inputstream.h"
#include "primitives.h"

#define MELTileSize 32

/**
 * Reference to the animation type.
 */
typedef struct melanimation MELAnimation;

/**
 * Reference to palette type.
 */
typedef struct melpalette MELPalette;

/**
 * Atlas listing the coordinates of sprites and tiles in a texture.
 */
typedef struct {
    MELTexture texture;
    int frameCount;
    MELRectangle * _Nullable frames;
    MELIntRectangle * _Nullable sources;
} MELTextureAtlas;

extern const MELTextureAtlas MELTextureAtlasEmpty;

/**
 * Returns an atlas by loading the texture and the atlas whose name starts with the given path.
 * The texture is expected to be named `path + ".bmp"` and the atlas to be named `path + ".atlas"`.
 *
 * @param path Path prefix common to the texture and its atlas.
 * @return An atlas.
 */
MELTextureAtlas MELTextureAtlasMakeWithPath(const char * _Nonnull path);

/**
 * Returns an atlas by reading the content of the given input stream.
 *
 * @param inputStream Data to read.
 * @param texture Texture represented by the atlas.
 * @return An atlas.
 */
MELTextureAtlas MELTextureAtlasMakeWithInputStreamAndTexture(MELInputStream * _Nonnull inputStream, MELTexture texture);

MELTextureAtlas MELTextureAtlasMakeWithPackMapAndRefList(MELPackMap packMap, MELRefList content);

/**
 * Returns an atlas by generating a texture using PackMap from every tile in the given palette.
 *
 * @param palette Palette to generate a texture and an atlas from.
 * @return An atlas with every tile from the given palette.
 */
MELTextureAtlas MELTextureAtlasMakeWithPalette(MELPalette * _Nonnull palette);

/**
 * Deinitialize and free the resources allocated for the given atlas.
 * The texture atlas itself is not freed.
 *
 * @param self Atlas instance.
 */
void MELTextureAtlasDeinit(MELTextureAtlas * _Nonnull self);

/**
 * Returns the origin and size of the current frame of the given animation in the texture atlas.
 *
 * @param self Atlas instance.
 * @param animation Animation instance.
 * @return A rectangle.
 */
MELRectangle MELTextureAtlasGetAnimationFrameRectangle(MELTextureAtlas self, MELAnimation * _Nonnull animation);

#endif /* textureatlas_h */
