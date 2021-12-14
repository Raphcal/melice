//
//  melstring.c
//  melice
//
//  Created by Raphaël Calabro on 09/11/2021.
//

#include "melstring.h"

#include <string.h>

MELListImplement(MELChar);
MELListImplement(MELChar16);
MELListImplement(MELCodePoint);

MELBoolean MELStringEquals(const char * _Nonnull lhs, const char * _Nonnull rhs) {
    return !strcmp(lhs, rhs);
}
uint64_t MELStringHash(const char * _Nonnull key) {
    uint64_t hash = 0;
    for (uint64_t index = 0; key[index]; index++) {
        hash = hash * index + key[index];
    }
    return hash;
}

char * _Nonnull MELStringCopy(const char * _Nullable source) {
    return source != NULL ? strdup(source) : NULL;
}

MELBoolean isUTF8Wagon(char * _Nonnull source, unsigned int index) {
    return ((source[index] & 0xFF) >> 6) == 2;
}

MELBoolean isTrailedByCountUTF8Wagon(char * _Nonnull source, unsigned int from, unsigned int count) {
    for (unsigned int index = 0; index < count; index++) {
        if (!isUTF8Wagon(source, from + index + 1)) {
            return false;
        }
    }
    return true;
}

MELCodePointList MELCodePointListMakeWithUTF8String(char * _Nullable source) {
    if (source == NULL) {
        return MELCodePointListEmpty;
    }
    MELCodePointList codePoints = MELCodePointListEmpty;
    MELCodePoint codePoint;
    uint32_t entry;
    for (unsigned int index = 0; source[index] != 0; index++) {
        entry = source[index] & 0xFF;
        if (entry <= 127) {
            // ASCII
            codePoint = entry;
        } else if (entry >> 5 == 6 && isUTF8Wagon(source, index + 1)) {
            // 2 bytes
            uint32_t byte1 = entry & 31;
            uint32_t byte2 = source[index + 1] & 63;
            codePoint = (byte1 << 6) | byte2;
            index++;
        } else if (entry >> 4 == 14 && isTrailedByCountUTF8Wagon(source, index, 2)) {
            // 3 bytes
            uint32_t byte1 = entry & 15;
            uint32_t byte2 = source[index + 1] & 63;
            uint32_t byte3 = source[index + 2] & 63;
            codePoint = (byte1 << 12) | (byte2 << 6) | byte3;
            index += 2;
        } else if (entry >> 3 == 30 && isTrailedByCountUTF8Wagon(source, index, 3)) {
            // 4 bytes
            uint32_t byte1 = entry & 7;
            uint32_t byte2 = source[index + 1] & 63;
            uint32_t byte3 = source[index + 2] & 63;
            uint32_t byte4 = source[index + 3] & 63;
            codePoint = (byte1 << 18) | (byte2 << 12) | (byte3 << 6) | byte4;
            index += 3;
        } else {
            // Encoding error
            codePoint = 0xFFFD;
        }
        MELCodePointListPush(&codePoints, codePoint);
    }
    return codePoints;
}

MELCodePointList MELCodePointListMakeWithUTF16String(uint16_t * _Nullable source) {
    if (source == NULL) {
        return MELCodePointListEmpty;
    }
    MELCodePointList codePoints = MELCodePointListEmpty;
    MELCodePoint codePoint;
    uint16_t entry, nextEntry;
    for (unsigned int index = 0; source[index] > 0; index++) {
        entry = source[index];
        nextEntry = source[index + 1];
        if (entry <= 0xD7FF || entry >= 0xE000) {
            // Basic Multilingual Plane
            codePoint = entry;
        } else if ((entry >= 0xD800 && entry <= 0xDBFF) && (nextEntry >= 0xDC00 && nextEntry <= 0xDFFF)) {
            // High surrogate - low surrogate
            codePoint = (entry & 0x3FF) << 10 | (nextEntry & 0x3FF) | 0x10000;
            index++;
        } else if ((entry >= 0xDC00 && entry <= 0xDFFF) && (nextEntry >= 0xD800 && nextEntry <= 0xDBFF)) {
            // Low surrogate - high surrogate
            codePoint = (nextEntry & 0x3FF) << 10 | (entry & 0x3FF) | 0x10000;
            index++;
        } else {
            // Encoding error
            codePoint = 0xFFFD;
        }
        MELCodePointListPush(&codePoints, codePoint);
    }
    return codePoints;
}

char * _Nullable MELUTF8StringMakeWithCodePoints(MELCodePointList codePoints) {
    MELCharList characters = MELCharListEmpty;
    for (unsigned int index = 0; index < codePoints.count; index++) {
        MELCodePoint codePoint = codePoints.memory[index];
        if (codePoint <= 0x007F) {
            MELCharListPush(&characters, codePoint);
        } else if (codePoint <= 0x07FF) {
            MELCharListPush(&characters, (codePoint >> 6) | 192);
            MELCharListPush(&characters, (codePoint & 63) | 128);
        } else if (codePoint <= 0xFFFF) {
            MELCharListPush(&characters, (codePoint >> 12) | 224);
            MELCharListPush(&characters, ((codePoint >> 6) & 63) | 128);
            MELCharListPush(&characters, (codePoint & 63) | 128);
        } else if (codePoint <= 0x10FFFF) {
            MELCharListPush(&characters, (codePoint >> 18) | 240);
            MELCharListPush(&characters, ((codePoint >> 12) & 63) | 128);
            MELCharListPush(&characters, ((codePoint >> 6) & 63) | 128);
            MELCharListPush(&characters, (codePoint & 63) | 128);
        } else {
            // Encoding error
            MELCharListPush(&characters, 0x3F);
        }
    }
    MELCharListPush(&characters, 0);
    return realloc(characters.memory, characters.count * sizeof(char));
}

char * _Nullable MELUTF8StringMakeWithUTF16String(uint16_t * _Nullable source) {
    if (source == NULL) {
        return NULL;
    }
    MELCodePointList codePoints = MELCodePointListMakeWithUTF16String(source);
    return MELUTF8StringMakeWithCodePoints(codePoints);
}

uint16_t * _Nullable MELUTF16StringMakeWithCodePoints(MELCodePointList codePoints) {
    MELChar16List characters = MELChar16ListEmpty;
    for (unsigned int index = 0; index < codePoints.count; index++) {
        MELCodePoint codePoint = codePoints.memory[index];
        if (codePoint <= 0xD7FF || (codePoint >= 0xE000 && codePoint <= 0xFFFF)) {
            // Basic Multilingual Plane
            MELChar16ListPush(&characters, codePoint);
        } else if (codePoint >= 0x010000 && codePoint <= 0x10FFFF) {
            // Supplementary Planes
            MELCodePoint valueToEncode = codePoint - 0x010000;
            // High surrogate
            MELChar16ListPush(&characters, (valueToEncode >> 10) | 0xD800);
            // Low surrogate
            MELChar16ListPush(&characters, (valueToEncode & 0x3FF) | 0xDC00);
        } else {
            // Encoding error
            MELChar16ListPush(&characters, 0xFFFD);
        }
    }
    MELChar16ListPush(&characters, 0);
    return realloc(characters.memory, characters.count * sizeof(uint16_t));
}

uint16_t * _Nullable MELUTF16StringMakeWithUTF8String(char * _Nullable source) {
    if (source == NULL) {
        return NULL;
    }
    MELCodePointList codePoints = MELCodePointListMakeWithUTF8String(source);
    return MELUTF16StringMakeWithCodePoints(codePoints);
}

