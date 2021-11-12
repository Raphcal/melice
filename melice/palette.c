//
//  palette.c
//  melice
//
//  Created by Raphaël Calabro on 02/11/2021.
//

#include "palette.h"

MELListImplement(MELPaletteRef);

size_t MELPaletteByteCount(MELIntSize tileSize, MELIntSize imageSize) {
    return sizeof(MELUInt32Color) * tileSize.width * imageSize.width * tileSize.height * imageSize.height;
}
