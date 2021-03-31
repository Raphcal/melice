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
    MELIntSize size;
    GLuint name;
} MELTexture;

MELTexture MELTextureMake(char * _Nonnull path);
void MELTextureDeinit(MELTexture * _Nonnull self);
void MELTextureBind(MELTexture self);
void MELTextureUnbind(MELTexture self);

#endif /* texture_h */
