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
#include "colorpalette.h"
#include "imagepalette.h"
#include "spriteinstance.h"

typedef struct melprojectformat MELProjectFormat;

typedef struct {
    MELBoolean (* _Nullable openProject)(MELProjectFormat * _Nonnull self, const char * _Nonnull path, MELProject * _Nonnull outProject);
    MELBoolean (* _Nullable saveProject)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, const char * _Nonnull path);

    MELPoint (* _Nullable readPoint)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    MELRectangle (* _Nullable readRectangle)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    MELUInt8Color (* _Nullable readColor)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    MELPaletteRef (* _Nullable readPalette)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    MELColorPalette * _Nullable (* _Nullable readColorPalette)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    MELImagePalette * _Nullable (* _Nullable readImagePalette)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    MELLayer (* _Nullable readLayer)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    MELMutableMap (* _Nullable readMap)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    MELSpriteDefinition (* _Nullable readSpriteDefinition)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    MELAnimationDefinition (* _Nullable readAnimationDefinition)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    MELSpriteInstance (* _Nullable readSpriteInstance)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
} MELProjectFormatClass;

typedef struct melprojectformat {
    MELProjectFormatClass * _Nonnull class;
    uint8_t * _Nonnull name;
    unsigned int version;
} MELProjectFormat;

#endif /* projectformat_h */
