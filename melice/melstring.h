//
//  melstring.h
//  melice
//
//  Created by Raphaël Calabro on 09/11/2021.
//

#ifndef melstring_h
#define melstring_h

#include "melstd.h"

MELBoolean MELStringEquals(const char * _Nonnull lhs, const char * _Nonnull rhs);

uint64_t MELStringHash(const char * _Nonnull key);

char * _Nonnull MELStringCopy(const char * _Nonnull source);

#endif /* melstring_h */
