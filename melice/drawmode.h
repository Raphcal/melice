//
//  drawmode.h
//  melice
//
//  Created by Raphaël Calabro on 28/12/2021.
//

#ifndef drawmode_h
#define drawmode_h

typedef enum {
    MELDrawModeUnset = 0,
    MELDrawModeTexture = 1,
    MELDrawModeColor = 2,
    MELDrawModeTextureAndColor = 3,
    MELDrawModeIndex = 4,
    MELDrawModeTextureAndIndex = 5,
    MELDrawModeColorAndIndex = 6,
    MELDrawModeTextureColorAndIndex = 7
} MELDrawMode;

#endif /* drawmode_h */
