//
//  primitives.h
//  melice
//
//  Created by Raphaël Calabro on 29/10/2021.
//

#ifndef primitives_h
#define primitives_h

#include "melstd.h"
#include "list.h"

typedef uintptr_t MELPointer;
typedef uint8_t MELByte;
typedef void * _Nonnull MELRef;

MELListDefine(GLfloat);
MELListDefine(GLubyte);
MELListDefine(GLint);
MELListDefine(MELByte);
MELListDefine(MELBoolean);
MELListDefine(MELRef);

void * _Nullable MELArrayCopy(const void * restrict _Nullable source, size_t size);

MELBoolean MELInt32ArrayIsFilledWithValue(const int32_t * _Nonnull self, size_t count, int32_t value);
MELBoolean MELUInt32ArrayIsFilledWithValue(const uint32_t * _Nonnull self, size_t count, uint32_t value);

#endif /* primitives_h */
