//
//  melstring.c
//  melice
//
//  Created by Raphaël Calabro on 09/11/2021.
//

#include "melstring.h"

#include <string.h>

MELBoolean MELStringEquals(const char * _Nonnull lhs, const char * _Nonnull rhs) {
    return strcmp(lhs, rhs) == 0;
}
uint64_t MELStringHash(const char * _Nonnull key) {
    uint64_t hash = 0;
    for (uint64_t index = 0; key[index]; index++) {
        hash = hash * index + key[index];
    }
    return hash;
}

char * _Nonnull MELStringCopy(const char * _Nonnull source) {
    size_t length = strlen(source) + 1;
    char *copy = malloc(sizeof(char) * length);
    strlcpy(copy, source, length);
    return copy;
}
