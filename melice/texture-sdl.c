//
//  texture.c
//  shmup
//
//  Created by Raphaël Calabro on 03/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "texture.h"

#include <SDL2/SDL.h>
#include <assert.h>

MELTexture MELTextureMake(char * _Nonnull path) {
    SDL_Surface *surface = SDL_LoadBMP(path);
    
    if (surface == NULL) {
        printf("SDL_LoadBMP failed: %s\n", SDL_GetError());
        return (MELTexture) { MELIntSizeZero, 0 };
    }
    
    MELTexture self;
    self.size = MELIntSizeMake(surface->w, surface->h);
    
    glGenTextures(1 , &self.name);
    glBindTexture(GL_TEXTURE_2D, self.name);

    SDL_LockSurface(surface);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    SDL_UnlockSurface(surface);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    SDL_FreeSurface(surface);
    
    return self;
}

void MELTextureDeinit(MELTexture * _Nonnull self) {
    self->size = MELIntSizeZero;
    glDeleteTextures(1, &self->name);
    self->name = 0;
}
