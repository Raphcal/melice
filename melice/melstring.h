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

typedef char * _Nullable MELString;
MELListDefine(MELString);
MELListDefineIndexOf(MELString);

typedef const char * _Nonnull MELConstString;
MELListDefine(MELConstString);
MELListDefineIndexOf(MELConstString);

MELBoolean MELStringEquals(const char * _Nonnull lhs, const char * _Nonnull rhs);

uint64_t MELStringHash(const char * _Nonnull key);

MELString MELStringCopy(const char * _Nullable restrict source);

void MELStringDeinit(MELString * _Nonnull self);

MELCodePointList MELCodePointListMakeWithUTF8String(MELString source);
MELCodePointList MELCodePointListMakeWithUTF16String(uint16_t * _Nullable source);

char * _Nullable MELUTF8StringMakeWithCodePoints(MELCodePointList codePoints);
char * _Nullable MELUTF8StringMakeWithUTF16String(uint16_t * _Nullable source);

uint16_t * _Nullable MELUTF16StringMakeWithCodePoints(MELCodePointList codePoints);
uint16_t * _Nullable MELUTF16StringMakeWithUTF8String(MELString source);

#endif /* melstring_h */
