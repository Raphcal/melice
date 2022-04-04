//
//  colorpalette.c
//  melice
//
//  Created by Raphaël Calabro on 01/11/2021.
//

#include "colorpalette.h"

#include "primitives.h"

#define MASK 1000

#define defaultColorCount 256
#define defaultAlphaLevelCount 8

const MELUInt8RGBColor defaultColors[] = {
    {0, 0, 6},
    {232, 250, 255},
    {235, 233, 248},
    {255, 238, 238},
    {253, 255, 214},
    {196, 255, 217},
    {0, 246, 245},
    {229, 254, 216},
    {210, 0, 0},
    {205, 245, 255},
    {219, 214, 245},
    {255, 207, 208},
    {250, 255, 127},
    {153, 248, 181},
    {0, 230, 228},
    {217, 254, 216},
    {0, 179, 0},
    {185, 240, 255},
    {202, 195, 235},
    {255, 172, 172},
    {248, 255, 0},
    {83, 238, 150},
    {0, 211, 210},
    {196, 255, 217},
    {161, 175, 0},
    {153, 234, 255},
    {189, 181, 231},
    {255, 139, 141},
    {251, 255, 0},
    {0, 229, 112},
    {0, 192, 190},
    {196, 254, 231},
    {45, 0, 183},
    {127, 228, 255},
    {173, 159, 228},
    {255, 83, 88},
    {242, 250, 0},
    {0, 218, 70},
    {0, 172, 170},
    {196, 253, 239},
    {212, 0, 179},
    {0, 219, 254},
    {156, 142, 216},
    {255, 0, 0},
    {235, 233, 0},
    {0, 213, 0},
    {0, 149, 148},
    {185, 253, 246},
    {0, 172, 170},
    {0, 198, 251},
    {143, 126, 211},
    {255, 0, 0},
    {231, 212, 0},
    {0, 200, 0},
    {0, 124, 123},
    {192, 246, 253},
    {217, 217, 217},
    {0, 185, 234},
    {129, 105, 198},
    {255, 0, 0},
    {233, 192, 0},
    {0, 186, 0},
    {0, 96, 93},
    {205, 233, 248},
    {202, 237, 214},
    {0, 173, 215},
    {113, 83, 194},
    {255, 0, 0},
    {225, 171, 0},
    {0, 172, 0},
    {255, 246, 216},
    {210, 227, 250},
    {169, 221, 255},
    {0, 158, 203},
    {103, 70, 187},
    {255, 0, 0},
    {215, 155, 0},
    {0, 155, 0},
    {255, 242, 201},
    {215, 221, 244},
    {154, 255, 218},
    {0, 145, 182},
    {88, 64, 164},
    {233, 0, 0},
    {202, 139, 0},
    {0, 139, 0},
    {255, 242, 193},
    {255, 250, 154},
    {98, 252, 213},
    {0, 118, 162},
    {72, 60, 138},
    {210, 0, 0},
    {191, 120, 0},
    {0, 129, 0},
    {255, 237, 178},
    {253, 246, 135},
    {31, 248, 207},
    {0, 102, 134},
    {67, 44, 120},
    {183, 0, 0},
    {176, 102, 0},
    {0, 112, 0},
    {255, 231, 169},
    {255, 239, 124},
    {0, 235, 210},
    {0, 79, 117},
    {42, 0, 89},
    {153, 0, 0},
    {163, 80, 0},
    {0, 87, 0},
    {255, 225, 162},
    {255, 234, 96},
    {0, 229, 212},
    {0, 54, 87},
    {8, 0, 66},
    {117, 0, 0},
    {143, 67, 0},
    {0, 63, 0},
    {255, 219, 145},
    {255, 229, 59},
    {0, 223, 214},
    {0, 23, 43},
    {5, 0, 26},
    {74, 0, 0},
    {133, 56, 0},
    {0, 46, 0},
    {255, 212, 134},
    {255, 223, 0},
    {255, 255, 255},
    {230, 238, 255},
    {255, 229, 255},
    {255, 235, 202},
    {225, 255, 229},
    {209, 255, 116},
    {255, 206, 124},
    {255, 216, 0},
    {245, 245, 245},
    {211, 227, 255},
    {255, 202, 251},
    {255, 219, 163},
    {200, 250, 202},
    {183, 255, 75},
    {255, 194, 105},
    {255, 204, 0},
    {233, 233, 233},
    {187, 214, 255},
    {255, 182, 247},
    {255, 200, 114},
    {174, 240, 182},
    {169, 245, 0},
    {255, 171, 100},
    {255, 189, 0},
    {223, 223, 223},
    {162, 201, 255},
    {254, 158, 237},
    {255, 180, 0},
    {142, 230, 160},
    {147, 241, 0},
    {255, 165, 89},
    {255, 181, 0},
    {211, 211, 211},
    {132, 188, 255},
    {243, 143, 226},
    {255, 156, 0},
    {113, 226, 131},
    {107, 230, 0},
    {251, 148, 83},
    {255, 166, 0},
    {193, 193, 193},
    {116, 171, 255},
    {243, 115, 215},
    {255, 127, 0},
    {54, 215, 115},
    {83, 218, 0},
    {239, 133, 76},
    {255, 147, 0},
    {180, 180, 180},
    {72, 157, 255},
    {232, 91, 210},
    {255, 129, 0},
    {0, 204, 92},
    {49, 205, 0},
    {227, 114, 65},
    {255, 255, 249},
    {167, 167, 167},
    {55, 137, 255},
    {220, 66, 197},
    {255, 120, 0},
    {0, 191, 68},
    {0, 199, 0},
    {213, 105, 52},
    {193, 193, 193},
    {152, 152, 152},
    {0, 133, 239},
    {215, 1, 184},
    {255, 112, 0},
    {0, 178, 31},
    {0, 255, 0},
    {209, 81, 42},
    {167, 167, 167},
    {137, 137, 137},
    {1, 117, 220},
    {199, 0, 171},
    {238, 104, 0},
    {0, 163, 0},
    {0, 244, 0},
    {192, 72, 18},
    {255, 0, 0},
    {120, 120, 120},
    {0, 112, 192},
    {184, 0, 163},
    {225, 79, 0},
    {0, 147, 0},
    {0, 233, 0},
    {177, 60, 0},
    {0, 255, 0},
    {103, 103, 103},
    {0, 96, 170},
    {175, 0, 147},
    {205, 54, 0},
    {0, 138, 0},
    {0, 219, 0},
    {160, 15, 0},
    {255, 255, 0},
    {81, 81, 81},
    {0, 71, 147},
    {156, 0, 129},
    {190, 0, 0},
    {0, 120, 0},
    {0, 207, 0},
    {141, 0, 0},
    {0, 0, 255},
    {43, 43, 43},
    {0, 65, 120},
    {131, 0, 120},
    {162, 0, 0},
    {0, 101, 0},
    {0, 193, 0},
    {130, 0, 0},
    {255, 0, 255},
    {2, 2, 2},
    {0, 34, 89},
    {119, 0, 99},
    {141, 0, 0},
    {0, 78, 0},
    {0, 180, 0},
    {104, 0, 0},
    {0, 255, 255},
    {0, 0, 6},
    {5, 0, 48},
    {92, 0, 74},
    {117, 0, 0},
    {0, 46, 0},
    {0, 163, 0},
    {74, 0, 0},
    {255, 255, 255}
};
const uint8_t defaultAlphaLevels[] = {255, 224, 192, 160, 128, 96, 64, 32};

MELColorPalette MELColorPaletteMakeDefault(void) {
    MELColorPalette self = {{&MELColorPaletteClass, strdup("Default color palette"), {1, 1}, 6, defaultColorCount}, NULL, NULL, defaultAlphaLevelCount};
    self.colors = MELArrayCopy(defaultColors, sizeof(MELUInt8RGBColor) * defaultColorCount);
    self.alphaLevels = MELArrayCopy(defaultAlphaLevels, sizeof(uint8_t) * defaultAlphaLevelCount);
    return self;
}

MELPalette * _Nonnull MELPaletteRefAllocDefaultColorPalette(void) {
    MELColorPalette *self = malloc(sizeof(MELColorPalette));
    *self = MELColorPaletteMakeDefault();
    return &self->super;
}

MELColorPalette MELColorPaletteMakeWithColorCount(size_t colorCount) {
    return (MELColorPalette) {
        {&MELColorPaletteClass, NULL, MELIntSizeMake(1, 1), 6, (uint32_t) colorCount},
        malloc(sizeof(MELUInt8RGBColor) * colorCount),
        MELArrayCopy(defaultAlphaLevels, sizeof(uint8_t) * defaultAlphaLevelCount),
        defaultAlphaLevelCount};
}

MELColorPalette MELColorPaletteMakeWithUInt32ColorList(MELUInt32ColorList colors) {
    MELColorPalette self;
    self.super.class = &MELColorPaletteClass;
    self.super.count = (unsigned int) colors.count;
    self.super.columns = 6;
    self.super.tileSize = MELIntSizeMake(1, 1);
    self.colors = malloc(sizeof(MELUInt8RGBColor) * colors.count);
    // TODO: Support alpha levels.
    self.alphaLevelCount = 1;
    self.alphaLevels = malloc(sizeof(uint8_t));
    self.alphaLevels[0] = 255;

    MELUInt8Color *uint8Colors = (MELUInt8Color *) colors.memory;
    for (unsigned int index = 0; index < self.super.count; index++) {
        MELUInt8Color color = uint8Colors[index];
        self.colors[index] = (MELUInt8RGBColor) {color.red, color.green, color.blue};
    }
    return self;
}

void MELColorPaletteDeinit(MELColorPalette * _Nonnull self) {
    free(self->colors);
    self->colors = NULL;
    free(self->alphaLevels);
    self->alphaLevels = NULL;
    self->alphaLevelCount = 0;
}

MELColorPalette * _Nonnull MELColorPaletteRefMakeWithColorPaletteRef(MELColorPalette * _Nonnull other) {
    MELColorPalette *self = malloc(sizeof(MELColorPalette));
    *self = *other;
    self->colors = MELArrayCopy(other->colors, sizeof(MELUInt8RGBColor) * other->super.count);
    self->alphaLevels = MELArrayCopy(other->alphaLevels, sizeof(uint8_t) * other->alphaLevelCount);
    return self;
}

MELUInt8Color MELColorPaletteUInt8ColorForTile(MELColorPalette self, unsigned int tileIndex) {
    const int colorIndex = tileIndex % MASK;
    const int alphaIndex = tileIndex / MASK;

    if (colorIndex < 0 || colorIndex >= self.super.count || alphaIndex < 0 || alphaIndex >= self.alphaLevelCount) {
        return MELUInt8ColorMake(0, 0, 0, 0);
    }

    MELUInt8RGBColor rgb = self.colors[colorIndex];
    uint8_t alpha = self.alphaLevels[alphaIndex];

    return MELUInt8ColorMake(rgb.red, rgb.green, rgb.blue, alpha);
}

MELUInt32Color MELColorPaletteColorForTile(MELColorPalette self, unsigned int tileIndex) {
    return MELUInt8ColorToRGBAUInt32Color(MELColorPaletteUInt8ColorForTile(self, tileIndex));
}

MELUInt32Color MELColorPaletteAlphaPremultipliedColorForTile(MELColorPalette self, unsigned int tileIndex) {
    MELUInt8Color color = MELColorPaletteUInt8ColorForTile(self, tileIndex);
    return MELUInt8ColorToRGBAUInt32Color(MELUInt8ColorMake(color.red * color.alpha / 255, color.green * color.alpha / 255, color.blue * color.alpha / 255, color.alpha));
}

int MELColorPaletteTileForColor(MELColorPalette self, MELUInt32Color color) {
    // TODO: Not implemented yet.
    return 0;
}

uint8_t * _Nullable MELColorPalettePaintTile(MELColorPalette * _Nonnull self, unsigned int tileIndex) {
    const MELIntSize tileSize = self->super.tileSize;

    MELUInt32Color color = MELColorPaletteColorForTile(*self, tileIndex);

    MELUInt32Color *pixels = malloc(MELPaletteByteCount(tileSize, MELIntSizeMake(1, 1)));
    const size_t count = tileSize.width * tileSize.height;
    for (size_t index = 0; index < count; index++) {
        pixels[index] = color;
    }
    return (uint8_t *) pixels;
}

void MELColorPalettePaintTileToBuffer(MELColorPalette * _Nonnull self, unsigned int tileIndex, MELIntPoint topLeft, MELUInt32Color * _Nonnull buffer, MELIntSize bufferSize) {
    const MELIntSize tileSize = self->super.tileSize;

    MELUInt32Color color = MELColorPaletteColorForTile(*self, tileIndex);

    const size_t count = tileSize.width * tileSize.height;
    for (size_t index = 0; index < count; index++) {
        const size_t x = index % tileSize.width;
        const size_t y = index / tileSize.width;
        buffer[topLeft.x + x + (topLeft.y + y) * bufferSize.width] = color;
    }
}

uint8_t * _Nullable MELColorPalettePaintImage(MELColorPalette * _Nonnull self, MELImagePaletteImage image, MELBoolean premultiplyAlpha) {
    MELColorPalette colorPalette = *self;

    MELUInt32Color *pixels = malloc(MELPaletteByteCount(image.size, MELIntSizeMake(1, 1)));

    const int *tiles = image.tiles;
    const size_t count = image.size.width * image.size.height;
    if (!premultiplyAlpha) {
        for (size_t index = 0; index < count; index++) {
            pixels[index] = MELColorPaletteColorForTile(colorPalette, tiles[index]);
        }
    } else {
        for (size_t index = 0; index < count; index++) {
            pixels[index] = MELColorPaletteAlphaPremultipliedColorForTile(colorPalette, tiles[index]);
        }
    }
    return (uint8_t *) pixels;
}

uint8_t * _Nullable MELColorPalettePaintMap(MELColorPalette * _Nonnull self, MELMap map, MELIntSize areaToRender) {
    // FIXME: This method does not paint the given map!
    const MELIntSize tileSize = self->super.tileSize;

    MELIntSize imageSize = MELIntSizeMake(areaToRender.width * tileSize.width, areaToRender.height * tileSize.height);
    const size_t count = imageSize.width * imageSize.height;

    MELUInt32Color *pixels = malloc(MELPaletteByteCount(tileSize, areaToRender));
    for (int index = 0; index < count; index++) {
        const int x = index % imageSize.width;
        const int y = index / imageSize.width;
        const int tileX = x / tileSize.width;
        const int tileY = y / tileSize.height;
        const int tileIndex = tileX + tileY * map.size.width;

        pixels[index] = MELColorPaletteColorForTile(*self, tileIndex);
    }
    return (uint8_t *) pixels;
}

MELUInt8RGBColor * _Nonnull MELColorPaletteTileAtIndex(MELColorPalette * _Nonnull self,  unsigned int tileIndex) {
    return self->colors + tileIndex % MASK;
}

const MELPaletteClass MELColorPaletteClass = {
    .initWithPaletteRef = (MELPaletteRef(*)(MELPaletteRef)) &MELColorPaletteRefMakeWithColorPaletteRef,
    .deinit = (void(*)(MELPalette *)) &MELColorPaletteDeinit,
    .paintTile = (uint8_t *(*)(MELPalette *, unsigned int)) &MELColorPalettePaintTile,
    .paintTileToBuffer = (void(*)(MELPalette *, unsigned int, MELIntPoint, MELUInt32Color *, MELIntSize))&MELColorPalettePaintTileToBuffer,
    .paintImage = (uint8_t *(*)(MELPalette *, MELImagePaletteImage, MELBoolean premultiplyAlpha))&MELColorPalettePaintImage,
    .paintMap = (uint8_t *(*)(MELPalette *, MELMap, MELIntSize)) &MELColorPalettePaintMap,
    .tileAtIndex = (void *(*)(MELPalette *, unsigned int)) &MELColorPaletteTileAtIndex
};
