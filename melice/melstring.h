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
    char * _Nullable bytes;
    size_t byteCount;
    size_t length;
    MELCharset charset;
} MELString;

MELString MELStringMake(char * _Nonnull cString);

uint64_t MELStringHash(MELString string);

#endif /* melstring_h */
