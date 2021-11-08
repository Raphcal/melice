//
//  projectformat.h
//  melice
//
//  Created by Raphaël Calabro on 02/11/2021.
//

#ifndef projectformat_h
#define projectformat_h

#include "melstd.h"

#include "project.h"

typedef struct melprojectformat MELProjectFormat;

typedef struct {
    MELProject (* _Nonnull openProject)(MELProjectFormat * _Nonnull self, char * _Nonnull path);
    void (* _Nonnull saveProject)(MELProjectFormat * _Nonnull self, char * _Nonnull path);
} MELProjectFormatClass;

typedef struct melprojectformat {
    MELProjectFormatClass * _Nonnull class;
    uint8_t * _Nonnull name;
} MELProjectFormat;

#endif /* projectformat_h */
