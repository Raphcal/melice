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

#include "size.h"

typedef struct {
    char * _Nullable path;
    MELIntSize size;
    GLuint name;
} MELTexture;

/**
 * Create a new texture instance for the bitmap file at the given path.
 *
 * @param path Path to the bitmap.
 */
MELTexture MELTextureMake(const char * _Nonnull path);

/**
 * Load the bitmap for this texture, generate an OpenGL texture and bind it.
 *
 * @param self Texture instance.
 */
void MELTextureLoad(MELTexture * _Nonnull self);

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
