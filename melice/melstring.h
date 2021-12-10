//
//  melstring.h
//  melice
//
//  Created by Raphaël Calabro on 09/11/2021.
//

#ifndef melstring_h
#define melstring_h

#include "melstd.h"

#include "list.h"

typedef char MELChar;
MELListDefine(MELChar);

typedef uint16_t MELChar16;
MELListDefine(MELChar16);

typedef uint32_t MELCodePoint;
MELListDefine(MELCodePoint);

MELBoolean MELStringEquals(const char * _Nonnull lhs, const char * _Nonnull rhs);

uint64_t MELStringHash(const char * _Nonnull key);

char * _Nullable MELStringCopy(const char * restrict _Nullable source);

MELCodePointList MELCodePointListMakeWithUTF8String(char * _Nullable source);
MELCodePointList MELCodePointListMakeWithUTF16String(uint16_t * _Nullable source);

char * _Nullable MELUTF8StringMakeWithCodePoints(MELCodePointList codePoints);
char * _Nullable MELUTF8StringMakeWithUTF16String(uint16_t * _Nullable source);

uint16_t * _Nullable MELUTF16StringMakeWithCodePoints(MELCodePointList codePoints);
uint16_t * _Nullable MELUTF16StringMakeWithUTF8String(char * _Nullable source);

#endif /* melstring_h */
