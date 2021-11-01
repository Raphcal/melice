//
//  melstring.h
//  melice
//
//  Created by Raphaël Calabro on 30/10/2021.
//

#ifndef melstring_h
#define melstring_h

#include "melstd.h"

#include "melcharset.h"

typedef struct {
    uint8_t * _Nullable bytes;
    size_t byteCount;
    int length;
    MELCharset charset;
} MELString;

uint64_t MELStringHash(MELString string);

#endif /* melstring_h */
