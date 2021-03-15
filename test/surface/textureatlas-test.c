//
//  textureatlas-test.c
//  shmup
//
//  Created by Raphaël Calabro on 04/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <melice.h>
#include <assert.h>

int main(int argc, char **argv) {
    MELFileManager *fileManager = MELFileManagerGetSharedInstance();
    MELFileManagerInitWithArguments(fileManager, argv);
    
    SDL_Window *window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    
    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    
    MELTextureAtlas atlas = MELTextureAtlasMakeWithPath("map0-texture");
    
    assert(atlas.texture.size.width == 256);
    assert(atlas.texture.size.height == 256);
    assert(atlas.frameCount == 16);
    
    MELTextureAtlasDeinit(&atlas);
    
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
