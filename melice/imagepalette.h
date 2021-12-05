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
#include "decorator.h"

extern const MELPaletteClass MELImagePaletteClass;

typedef struct {
    int * _Nullable tiles;
    MELDecoratorRefList decorators;
} MELImagePaletteImage;

typedef struct {
    MELPalette super;
    MELColorPalette * _Nonnull colorPalette;
    MELImagePaletteImage * _Nullable images;
} MELImagePalette;

MELListDefine(MELImagePaletteImage);

/**
 * Deinitialize the given palette and free its images.
 *
 * Color palettes are not freed because it is assumed that they are owned by an other object.
 *
 * @param self Image palette to deinitialize.
 */
void MELImagePaletteDeinit(MELImagePalette * _Nonnull self);

void MELImagePaletteImageDeinit(MELImagePaletteImage * _Nonnull self);

#endif /* imagepalette_h */
