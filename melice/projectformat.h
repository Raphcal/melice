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
#include "inputstream.h"
#include "outputstream.h"

typedef struct melprojectformat MELProjectFormat;

typedef struct {
    MELBoolean (* _Nullable openProject)(MELProjectFormat * _Nonnull self, MELInputStream * _Nonnull inputStream, MELProject * _Nonnull outProject);
    MELBoolean (* _Nullable openProjectAtPath)(MELProjectFormat * _Nonnull self, const char * _Nonnull path, MELProject * _Nonnull outProject);
    uint8_t * _Nonnull (* _Nullable saveProject)(MELProjectFormat * _Nonnull self, MELProject project, size_t * _Nonnull outCount);
    MELBoolean (* _Nullable saveProjectAtPath)(MELProjectFormat * _Nonnull self, MELProject project, const char * _Nonnull path);

    MELIntRectangle (* _Nullable readRectangle)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    void (* _Nullable writeRectangle)(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELIntRectangle rectangle);

    MELUInt8Color (* _Nullable readColor)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    void (* _Nullable writeColor)(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELUInt8Color color);

    MELPaletteRef (* _Nullable readPalette)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    void (* _Nullable writePalette)(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELPaletteRef palette);

    MELColorPalette * _Nullable (* _Nullable readColorPalette)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    void (* _Nullable writeColorPalette)(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELColorPalette * _Nonnull colorPalette);

    MELImagePalette * _Nullable (* _Nullable readImagePalette)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    void (* _Nullable writeImagePalette)(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELImagePalette * _Nonnull imagePalette);

    MELImagePaletteImage (* _Nullable readImagePaletteImage)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    void (* _Nullable writeImagePaletteImage)(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELImagePaletteImage imagePaletteImage);

    MELPaletteRef (* _Nullable readPaletteReference)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    void (* _Nullable writePaletteReference)(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, unsigned int paletteIndex);

    MELPoint (* _Nullable readScrollRate)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    void (* _Nullable writeScrollRate)(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELPoint scrollRate);

    MELLayer (* _Nullable readLayer)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    void (* _Nullable writeLayer)(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELLayer layer);

    MELMutableMap (* _Nullable readMap)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    void (* _Nullable writeMap)(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELMutableMap map);

    MELSpriteDefinition (* _Nullable readSpriteDefinition)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    void (* _Nullable writeSpriteDefinition)(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELSpriteDefinition spriteDefinition);

    MELAnimationDefinition (* _Nullable readAnimationDefinition)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    void (* _Nullable writeAnimationDefinition)(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELAnimationDefinition animationDefinition);

    MELSpriteInstance (* _Nullable readSpriteInstance)(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream);
    void (* _Nullable writeSpriteInstance)(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELSpriteInstance spriteInstance);
} MELProjectFormatClass;

typedef struct melprojectformat {
    const MELProjectFormatClass * _Nonnull class;
    char * _Nonnull name;
    unsigned int version;
} MELProjectFormat;

#endif /* projectformat_h */
