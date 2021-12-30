//
//  imagepalette.h
//  melice
//
//  Created by Raphaël Calabro on 01/11/2021.
//

#ifndef imagepalette_h
#define imagepalette_h

#include "melstd.h"

#include "palette.h"
#include "colorpalette.h"
#include "imagepaletteimage.h"

extern const MELPaletteClass MELImagePaletteClass;

typedef struct {
    MELPalette super;
    MELColorPalette * _Nonnull colorPalette;
    MELImagePaletteImage * _Nullable images;
} MELImagePalette;

extern const MELImagePalette MELImagePaletteEmpty;

MELImagePalette MELImagePaletteMakeWithColorPalette(MELColorPalette * _Nonnull colorPalette);

/**
 * NOTE: The colorPalette is not duplicated and will be shared by both instances..
 */
MELImagePalette * _Nonnull MELImagePaletteRefMakeWithMELImagePaletteRef(MELImagePalette * _Nonnull other);

/**
 * Deinitialize the given palette and free its images.
 *
 * Color palettes are not freed because it is assumed that they are owned by an other object.
 *
 * @param self Image palette to deinitialize.
 */
void MELImagePaletteDeinit(MELImagePalette * _Nonnull self);

#endif /* imagepalette_h */
