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

MELListDefine(GLfloat);
MELListDefine(GLubyte);
MELListDefine(MELByte);

void * _Nullable MELArrayCopy(const void * restrict _Nullable source, size_t size);

#endif /* primitives_h */
