//
//  mmkprojectformat.h
//  melice
//
//  Created by Raphaël Calabro on 07/11/2021.
//

#ifndef mmkprojectformat_h
#define mmkprojectformat_h

#include "melstd.h"

#include "projectformat.h"

typedef struct {
    MELProjectFormat super;
    uint8_t version;
} MELMmkProjectFormat;

extern const MELProjectFormatClass MELMmkProjectFormatClass;

#endif /* mmkprojectformat_h */
