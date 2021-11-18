//
//  texture.h
//  shmup
//
//  Created by Raphaël Calabro on 03/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef texture_h
#define texture_h

#include "melstd.h"

#include "color.h"
#include "size.h"

/// MELPackMap reference.
typedef struct melpackmap MELPackMap;

typedef struct {
    char * _Nullable path;
    MELUInt32Color * _Nullable pixels;
    MELIntSize size;
    GLuint name;
} MELTexture;

extern const MELTexture MELTextureEmpty;

/**
 * Create a new texture instance for the bitmap file at the given path.
 *
 * @param path Path to the bitmap.
 */
MELTexture MELTextureMake(const char * _Nonnull path);

/**
 * Create a new texture instance for the bitmap file with the given folder in the current asset folder.
 *
 * @param asset Name of the bitmap file.
 */
MELTexture MELTextureMakeWithAsset(const char * _Nonnull asset);

MELTexture MELTextureMakeWithPackMap(MELPackMap packMap);

/**
 * Load the bitmap for this texture, generate an OpenGL texture and bind it.
 *
 * @param self Texture instance.
 */
void MELTextureLoad(MELTexture * _Nonnull self);

/**
 * Free and destroy the texture but does not the path to the texture to be able to load it again later.
 */
void MELTextureUnload(MELTexture * _Nonnull self);

/**
 * Bind the given texture. It should have been loaded before using `MELTextureLoad(MELTexture *)`.
 *
 * @param self Texture to bind.
 */
void MELTextureBind(MELTexture self);

/**
 * Deinit the given texture by freeing the path and destroying the texture from the OpenGL context if it had been loaded.
 *
 * @param self Texture instance to deinit.
 */
void MELTextureDeinit(MELTexture * _Nonnull self);

#endif /* texture_h */
