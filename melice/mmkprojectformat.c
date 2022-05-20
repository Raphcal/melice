//
//  mmkprojectformat.c
//  melice
//
//  Created by Raphaël Calabro on 07/11/2021.
//

#include "mmkprojectformat.h"

#include "../dependencies/miniz/miniz.h"
#include "filemanager.h"
#include "melstring.h"
#include "melmath.h"

#define LAST_SUPPORTED_VERSION 12
#define DATA_ENTRY "data"

const MELProjectFormat MELMmkProjectFormat = {&MELMmkProjectFormatClass, NULL, LAST_SUPPORTED_VERSION};

MELBoolean openFileOrArchiveAtPath(const char * _Nonnull path, MELInputStream * _Nonnull outInputStream) {
    MELInputStream inputStream = MELInputStreamOpen(path);
    if (inputStream.file == NULL) {
        fprintf(stderr, "Unable to open file at path: %s\n", path);
        return false;
    }
    uint8_t header[4];
    MELInputStreamRead(&inputStream, header, sizeof(uint8_t) * 4);

    const uint8_t zipHeader[] = {0x50, 0x4b, 0x03, 0x04};
    if (memcmp(header, zipHeader, sizeof(uint8_t) * 4) != 0) {
        if (MELInputStreamSeek(&inputStream, 0, MELInputStreamSeekFromStart)) {
            MELInputStreamClose(&inputStream);
            inputStream = MELInputStreamOpen(path);
            if (!inputStream.file) {
                fprintf(stderr, "Unable to rewind/reopen file at path: %s\n", path);
                return false;
            }
        }
        *outInputStream = inputStream;
        return true;
    }
    MELInputStreamClose(&inputStream);

    mz_zip_archive archive = {};
    memset(&archive, 0, sizeof(mz_zip_archive));
    if (!mz_zip_reader_init_file(&archive, path, 0)) {
        mz_zip_error error = mz_zip_get_last_error(&archive);
        fprintf(stderr, "Unable to open zip file at path: %s, zip error: %s\n", path, mz_zip_get_error_string(error));
        return false;
    }
    size_t size;
    void * _Nullable data = mz_zip_reader_extract_file_to_heap(&archive, DATA_ENTRY, &size, 0);
    mz_zip_reader_end(&archive);
    if (data == NULL) {
        mz_zip_error error = mz_zip_get_last_error(&archive);
        fprintf(stderr, "Unable to extract data of MMK file at path: %s, zip error: %s\n", path, mz_zip_get_error_string(error));
        return false;
    }
    *outInputStream = MELInputStreamInitWithBytes(data, size);
    mz_free(data);
    return true;
}

MELBoolean MELMmkProjectFormatOpenProject(MELProjectFormat * _Nonnull self, MELInputStream * _Nonnull inputStream, MELProject * _Nonnull outProject) {
    uint16_t header[5];
    MELInputStreamRead(inputStream, header, sizeof(uint16_t) * 4);
    header[4] = 0;
    char *utf8Header = MELUTF8StringMakeWithUTF16String(header);

    unsigned int version;
    if (memcmp(utf8Header, "MMK", 3) == 0) {
        version = header[3] - '0';
    } else if (memcmp(utf8Header, "MM", 2) == 0) {
        version = (header[2] - '0') * 10 + header[3] - '0';
    } else {
        version = 1;
        inputStream->cursor = 0;
    }
    free(utf8Header);

    if (version > LAST_SUPPORTED_VERSION) {
        fprintf(stderr, "Project version %d is unsupported, last supported version is %d\n", version, LAST_SUPPORTED_VERSION);
        return false;
    }

    self->version = version;

    MELProject project = MELProjectMake();

    // Loading palettes.
    MELBoolean hasDefaultColorPalette = false;
    const int paletteCount = MELInputStreamReadInt(inputStream);
    for (int index = 0; index < paletteCount; index++) {
        MELPaletteRef palette = self->class->readPalette(self, &project, inputStream);
        MELPaletteRefListPush(&project.palettes, palette);

        hasDefaultColorPalette = hasDefaultColorPalette || (MELPaletteIsColorPalette(palette) && palette->count == 256);
    }
    if (!hasDefaultColorPalette) {
        MELPaletteRefListPush(&project.palettes, MELPaletteRefAllocDefaultColorPalette());
    }

    // Loading maps.
    const int mapCount = MELInputStreamReadInt(inputStream);
    for (int index = 0; index < mapCount; index++) {
        MELMutableMapListPush(&project.root.maps, self->class->readMap(self, &project, inputStream));
    }

    if (version >= 3) {
        // Loading sprites.
        const int spriteCount = MELInputStreamReadInt(inputStream);
        for (int index = 0; index < spriteCount; index++) {
            MELSpriteDefinitionListPush(&project.root.sprites, self->class->readSpriteDefinition(self, &project, inputStream));
        }

        // Loading sprite instances.
        const MELPoint defaultScrollRate = MELPointMake(1, 1);
        for (int mapIndex = 0; mapIndex < mapCount; mapIndex++) {
            MELLayerList * _Nonnull layers = &project.root.maps.memory[mapIndex].super.layers;
            MELLayer * _Nullable layer = NULL;
            for (int layerIndex = 0; layerIndex < layers->count; layerIndex++) {
                if (strcmp(layers->memory[layerIndex].name, "Piste") == 0) {
                    layer = layers->memory + layerIndex;
                    break;
                }
                if (layer != NULL && MELPointEquals(layers->memory[layerIndex].scrollRate, defaultScrollRate)) {
                    layer = layers->memory + layerIndex;
                }
            }
            if (layer == NULL && layers->count > 0) {
                layer = layers->memory;
            }

            const int instanceCount = MELInputStreamReadInt(inputStream);
            for (int instanceIndex = 0; instanceIndex < instanceCount; instanceIndex++) {
                MELSpriteInstance instance = self->class->readSpriteInstance(self, &project, inputStream);
                if (layer != NULL) {
                    MELSpriteInstanceListPush(&layer->sprites, instance);
                }
            }
        }
    }

    *outProject = project;
    return true;
}

MELBoolean MELMmkProjectFormatOpenProjectAtPath(MELProjectFormat * _Nonnull self, const char * _Nonnull path, MELProject * _Nonnull outProject) {
    MELInputStream inputStream;
    if (!openFileOrArchiveAtPath(path, &inputStream)) {
        return false;
    }
    MELBoolean status = MELMmkProjectFormatOpenProject(self, &inputStream, outProject);
    MELInputStreamClose(&inputStream);
    return status;
}

void writeProject(MELProjectFormat * _Nonnull self, MELOutputStream * _Nonnull outputStream, MELProject project) {
    uint16_t header[4] = {'M', 'M', 'K', '0'};
    if (self->version < 10) {
        header[3] = '0' + self->version;
    } else {
        header[2] = '0' + (self->version / 10);
        header[3] = '0' + (self->version % 10);
    }
    MELOutputStreamWrite(outputStream, header, sizeof(uint16_t) * 4);

    // Writing palettes.
    MELOutputStreamWriteInt(outputStream, (int32_t) project.palettes.count);
    for (unsigned int index = 0; index < project.palettes.count; index++) {
        self->class->writePalette(self, project, outputStream, project.palettes.memory[index]);
    }

    // Writing maps.
    MELOutputStreamWriteInt(outputStream, (int32_t) project.root.maps.count);
    for (unsigned int index = 0; index < project.root.maps.count; index++) {
        self->class->writeMap(self, project, outputStream, project.root.maps.memory[index]);
    }

    if (self->version >= 3) {
        // Writing sprites definitions.
        MELSpriteDefinitionList sprites = project.root.sprites;
        MELOutputStreamWriteInt(outputStream, (int32_t) sprites.count);
        for (unsigned int index = 0; index < sprites.count; index++) {
            self->class->writeSpriteDefinition(self, project, outputStream, sprites.memory[index]);
        }
        
        // Writing sprites instances.
        for (unsigned int mapIndex = 0; mapIndex < project.root.maps.count; mapIndex++) {
            const MELMutableMap map = project.root.maps.memory[mapIndex];
            MELSpriteInstanceList instances = MELSpriteInstanceListEmpty;
            for (unsigned int layerIndex = 0; layerIndex < map.super.layers.count; layerIndex++) {
                MELSpriteInstanceListAddAll(&instances, map.super.layers.memory[layerIndex].sprites);
            }

            MELOutputStreamWriteInt(outputStream, (int32_t) instances.count);
            for (unsigned int index = 0; index < instances.count; index++) {
                self->class->writeSpriteInstance(self, project, outputStream, instances.memory[index]);
            }
        }
    }
}

uint8_t * _Nonnull MELMmkProjectFormatSaveProject(MELProjectFormat * _Nonnull self, MELProject project, size_t * _Nonnull outCount) {
    MELOutputStream outputStream = MELOutputStreamInit();

    writeProject(self, &outputStream, project);

    uint8_t *bytes = MELOutputStreamGetBytes(outputStream);
    *outCount = outputStream.count;
    MELOutputStreamDeinit(&outputStream);
    return bytes;
}

MELBoolean MELMmkProjectFormatSaveProjectAtPath(MELProjectFormat * _Nonnull self, MELProject project, const char * _Nonnull path) {
    MELOutputStream outputStream = MELOutputStreamOpen(path);
    if (!outputStream.file) {
        MELOutputStreamDeinit(&outputStream);
        return false;
    }

    writeProject(self, &outputStream, project);
    MELOutputStreamClose(&outputStream);
    return true;
}

MELIntRectangle MELMmkProjectFormatReadRectangle(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    MELIntRectangle rectangle;
    if (MELInputStreamReadBoolean(inputStream)) {
        const int32_t x = MELInputStreamReadInt(inputStream);
        const int32_t y = MELInputStreamReadInt(inputStream);
        const int32_t width = MELInputStreamReadInt(inputStream);
        const int32_t height = MELInputStreamReadInt(inputStream);
        rectangle = MELIntRectangleMake(x, y, width, height);
    } else {
        rectangle = MELIntRectangleZero;
    }
    return rectangle;
}
void MELMmkProjectFormatWriteRectangle(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELIntRectangle rectangle) {
    MELOutputStreamWriteBoolean(outputStream, true);
    MELOutputStreamWriteInt(outputStream, rectangle.origin.x);
    MELOutputStreamWriteInt(outputStream, rectangle.origin.y);
    MELOutputStreamWriteInt(outputStream, rectangle.size.width);
    MELOutputStreamWriteInt(outputStream, rectangle.size.height);
}

MELUInt8Color MELMmkProjectFormatReadColor(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    const int red = MELInputStreamReadInt(inputStream);
    const int green = MELInputStreamReadInt(inputStream);
    const int blue = MELInputStreamReadInt(inputStream);
    const int alpha = MELInputStreamReadInt(inputStream);
    return MELUInt8ColorMake(red, green, blue, alpha);
}
void MELMmkProjectFormatWriteColor(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELUInt8Color color) {
    MELOutputStreamWriteInt(outputStream, color.red);
    MELOutputStreamWriteInt(outputStream, color.green);
    MELOutputStreamWriteInt(outputStream, color.blue);
    MELOutputStreamWriteInt(outputStream, color.alpha);
}

MELPaletteRef MELMmkProjectFormatReadBufferedImagePalette(MELInputStream * _Nonnull inputStream) {
    MELIntSize tileSize;
    tileSize.width = tileSize.height = MELInputStreamReadInt(inputStream);
    MELIntSize size = MELIntSizeMake(MELInputStreamReadInt(inputStream), MELInputStreamReadInt(inputStream));
    MELIntSize tileCount = MELIntSizeMake(size.width / tileSize.width, size.height / tileSize.height);
    int32_t count = MELInputStreamReadInt(inputStream);
    int *atlas = malloc(sizeof(int) * count);

    // TODO: Use a set or a sorted list instead.
    MELUInt32ColorList palette = MELUInt32ColorListEmpty;

    for (unsigned int index = 0; index < count; index++) {
        uint32_t pixel = MELInputStreamReadUInt32(inputStream);
        MELUInt8BGRAColor bgraColor = *((MELUInt8BGRAColor *)&pixel);
        MELUInt32Color rgbaColor = MELUInt8ColorToRGBAUInt32Color((MELUInt8Color) {bgraColor.red, bgraColor.green, bgraColor.blue, bgraColor.alpha});
        int tile = MELUInt32ColorListIndexOf(palette, rgbaColor);
        if (tile == -1) {
            tile = (int) palette.count;
            MELUInt32ColorListPush(&palette, rgbaColor);
        }
        atlas[index] = tile;
    }

    MELColorPalette *colorPalette = malloc(sizeof(MELColorPalette));
    *colorPalette = MELColorPaletteMakeWithUInt32ColorList(palette);

    const int imageCount = tileCount.width * tileCount.height;

    MELImagePalette *imagePalette = malloc(sizeof(MELImagePalette));
    imagePalette->super.tileSize = tileSize;
    imagePalette->super.count = imageCount;
    imagePalette->images = malloc(sizeof(MELImagePaletteImage) * imageCount);
    imagePalette->colorPalette = colorPalette;

    const int imageTileCount = tileSize.width * tileSize.height;
    for (unsigned int imageIndex = 0; imageIndex < imageCount; imageIndex++) {
        MELImagePaletteImage image = (MELImagePaletteImage) {malloc(sizeof(int) * imageTileCount), tileSize, MELDecoratorRefListEmpty};
        const int imageX = imageIndex / tileCount.width;
        const int imageY = imageIndex % tileCount.width;
        int *row = atlas + imageX * tileSize.width + imageY * tileSize.height * tileCount.width;
        for (unsigned int y = 0; y < tileSize.height; y++) {
            memcpy(image.tiles + y * tileSize.width, row, sizeof(int) * tileSize.width);
            row += tileSize.width * tileCount.width;
        }
        imagePalette->images[imageIndex] = image;
    }

    return &imagePalette->super;
}

MELPaletteRef MELMmkProjectFormatReadPalette(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    MELPaletteRef palette = NULL;
    char *paletteClass = MELInputStreamReadString(inputStream);
    if (strcmp(paletteClass, "fr.rca.mapmaker.model.palette.AlphaColorPalette") == 0) {
        palette = &self->class->readColorPalette(self, project, inputStream)->super;
    } else if (strcmp(paletteClass, "fr.rca.mapmaker.model.palette.EditableImagePalette") == 0) {
        palette = &self->class->readImagePalette(self, project, inputStream)->super;
    } else if (strcmp(paletteClass, "fr.rca.mapmaker.model.palette.PaletteReference") == 0) {
        palette = self->class->readPaletteReference(self, project, inputStream);
    } else if (strcmp(paletteClass, "fr.rca.mapmaker.model.palette.ImagePalette") == 0) {
        palette = MELMmkProjectFormatReadBufferedImagePalette(inputStream);
    } else {
        fprintf(stderr, "Unsupported palette class: %s\n", paletteClass);
    }
    free(paletteClass);
    return palette;
}

void MELMmkProjectFormatWritePalette(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELPaletteRef palette) {
    if (palette->class == &MELColorPaletteClass) {
        MELOutputStreamWriteString(outputStream, "fr.rca.mapmaker.model.palette.AlphaColorPalette");
        self->class->writeColorPalette(self, project, outputStream, (MELColorPalette *)palette);
    } else if (palette->class == &MELImagePaletteClass) {
        MELOutputStreamWriteString(outputStream, "fr.rca.mapmaker.model.palette.EditableImagePalette");
        self->class->writeImagePalette(self, project, outputStream, (MELImagePalette *)palette);
    }
}

void MELMmkProjectFormatWritePaletteAsReference(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELPaletteRef palette) {
    for (unsigned int index = 0; index < project.palettes.count; index++) {
        if (palette == project.palettes.memory[index]) {
            MELOutputStreamWriteString(outputStream, "fr.rca.mapmaker.model.palette.PaletteReference");
            self->class->writePaletteReference(self, project, outputStream, index);
            return;
        }
    }
    fprintf(stderr, "Unable to find index of given palette\n");
    MELMmkProjectFormatWritePalette(self, project, outputStream, palette);
}

MELColorPalette * _Nullable MELMmkProjectFormatReadColorPalette(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    const int count = MELInputStreamReadInt(inputStream);

    MELColorPalette colorPalette = MELColorPaletteMakeWithColorCount(count);

    MELUInt8Color color;
    for (int index = 0; index < count; index++) {
        color = self->class->readColor(self, project, inputStream);
        colorPalette.colors[index] = (MELUInt8RGBColor) {color.red, color.green, color.blue};
    }

    MELColorPalette *result = malloc(sizeof(MELColorPalette));
    *result = colorPalette;
    return result;
}

void MELMmkProjectFormatWriteColorPalette(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELColorPalette * _Nonnull colorPalette) {
    const MELUInt8RGBColor *colors = colorPalette->colors;

    MELOutputStreamWriteInt(outputStream, colorPalette->super.count);
    for (unsigned int index = 0; index < colorPalette->super.count; index++) {
        const MELUInt8RGBColor color = colors[index];
        const MELUInt8Color rgba = (MELUInt8Color) {color.red, color.green, color.blue, 0xFF};
        self->class->writeColor(self, project, outputStream, rgba);
    }
}

MELImagePalette * _Nullable MELMmkProjectFormatReadImagePalette(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    char *name = MELInputStreamReadString(inputStream);

    const int tileSize = MELInputStreamReadInt(inputStream);
    int32_t columns = MELInputStreamReadInt(inputStream);

    MELColorPalette *colorPalette = self->class->readColorPalette(self, project, inputStream);

    const int count = MELInputStreamReadInt(inputStream);

    MELImagePalette imagePalette = {{&MELImagePaletteClass, name, MELIntSizeMake(tileSize, tileSize), (uint8_t) MELIntBound(0, columns, 0xFF), count}, colorPalette, malloc(sizeof(MELImagePaletteImage) * count)};

    for (int index = 0; index < count; index++) {
        MELImagePaletteImage image = self->class->readImagePaletteImage(self, project, inputStream);

        if (self->version >= 3) {
            char *function = MELInputStreamReadNullableString(inputStream);
            if (function != NULL) {
                MELFunctionDecorator *functionDecorator = malloc(sizeof(MELFunctionDecorator));
                *functionDecorator = (MELFunctionDecorator) {{MELDecoratorTypeFunction}, function};
                MELDecoratorRefListPush(&image.decorators, &functionDecorator->super);
            }
        }

        imagePalette.images[index] = image;
    }

    MELImagePalette *result = malloc(sizeof(MELImagePalette));
    *result = imagePalette;
    return result;
}

void MELMmkProjectFormatWriteImagePalette(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELImagePalette * _Nonnull imagePalette) {
    MELOutputStreamWriteString(outputStream, imagePalette->super.name);
    MELOutputStreamWriteInt(outputStream, imagePalette->super.tileSize.width);
    MELOutputStreamWriteInt(outputStream, imagePalette->super.columns);

    self->class->writeColorPalette(self, project, outputStream, imagePalette->colorPalette);

    const uint32_t count = imagePalette->super.count;
    MELOutputStreamWriteInt(outputStream, count);

    MELImagePaletteImage *images = imagePalette->images;
    for (unsigned int index = 0; index < count; index++) {
        MELImagePaletteImage image = images[index];
        self->class->writeImagePaletteImage(self, project, outputStream, image);

        if (self->version >= 3) {
            char *function = NULL;
            for (unsigned int decoratorIndex = 0; decoratorIndex < image.decorators.count; decoratorIndex++) {
                if (image.decorators.memory[decoratorIndex]->type == MELDecoratorTypeFunction) {
                    MELFunctionDecorator *functionDecorator = (MELFunctionDecorator *) image.decorators.memory[decoratorIndex];
                    function = functionDecorator->function;
                    break;
                }
            }
            MELOutputStreamWriteNullableString(outputStream, function);
        }
    }
}

MELImagePaletteImage MELMmkProjectFormatReadImagePaletteImage(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    MELLayer layer = self->class->readLayer(self, project, inputStream);
    free(layer.name);
    return (MELImagePaletteImage) {layer.tiles, layer.size, MELDecoratorRefListEmpty};
}

void MELMmkProjectFormatWriteImagePaletteImage(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELImagePaletteImage image) {
    MELLayer layer = (MELLayer) {NULL, image.size, image.size.width * image.size.height, image.tiles, MELPointZero, false, MELSpriteInstanceListEmpty};
    self->class->writeLayer(self, project, outputStream, layer);
}

MELPaletteRef MELMmkProjectFormatReadPaletteReference(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    const int paletteIndex = MELInputStreamReadInt(inputStream);
    return project->palettes.memory[paletteIndex];
}

void MELMmkProjectFormatWritePaletteReference(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, unsigned int paletteIndex) {
    MELOutputStreamWriteInt(outputStream, paletteIndex);
}

MELPoint MELMmkProjectFormatReadScrollRate(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    const float x = MELInputStreamReadFloat(inputStream);
    const float y = self->version < 5 ? x : MELInputStreamReadFloat(inputStream);
    return MELPointMake(x, y);
}

void MELMmkProjectFormatWriteScrollRate(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELPoint scrollRate) {
    MELOutputStreamWriteFloat(outputStream, (float) scrollRate.x);
    if (self->version >= 5) {
        MELOutputStreamWriteFloat(outputStream, (float) scrollRate.y);
    }
}

MELLayer MELMmkProjectFormatReadLayer(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    char *name = MELInputStreamReadString(inputStream);
    const int width = MELInputStreamReadInt(inputStream);
    const int height = MELInputStreamReadInt(inputStream);

    MELPoint scrollRate = self->class->readScrollRate(self, project, inputStream);

    MELBoolean isSolid;
    if (self->version >= 12) {
        isSolid = MELInputStreamReadBoolean(inputStream);
    } else {
        isSolid = name[0] == 'P';
    }

    int32_t count;
    int32_t *tiles = MELInputStreamReadIntArray(inputStream, &count);

    return (MELLayer) {name, MELIntSizeMake(width, height), count, tiles, scrollRate, isSolid, MELSpriteInstanceListEmpty};
}

void MELMmkProjectFormatWriteLayer(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELLayer layer) {
    MELOutputStreamWriteString(outputStream, layer.name != NULL ? layer.name : "");
    MELOutputStreamWriteInt(outputStream, layer.size.width);
    MELOutputStreamWriteInt(outputStream, layer.size.height);
    self->class->writeScrollRate(self, project, outputStream, layer.scrollRate);
    if (self->version >= 12) {
        MELOutputStreamWriteBoolean(outputStream, layer.isSolid);
    }
    MELOutputStreamWriteIntArray(outputStream, layer.tiles, layer.tileCount);
}

MELMutableMap MELMmkProjectFormatReadMap(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    MELMutableMap map;
    if (self->version >= 7) {
        /* index */ MELInputStreamReadInt(inputStream);
    }
    if (self->version >= 6) {
        map.name = MELInputStreamReadNullableString(inputStream);
    }
    map.super.backgroundColor = MELColorMakeWithMELUInt8Color(self->class->readColor(self, project, inputStream));
    map.palette = self->class->readPalette(self, project, inputStream);

    map.super.size = MELIntSizeZero;

    const int layerCount = MELInputStreamReadInt(inputStream);
    map.super.layers = MELLayerListMakeWithInitialCapacity(layerCount);
    for (int index = 0; index < layerCount; index++) {
        MELLayer layer = self->class->readLayer(self, project, inputStream);
        MELLayerListPush(&map.super.layers, layer);

        int32_t layerWidth = layer.size.width / MELFloatMax(layer.scrollRate.x, 1.0f);
        int32_t layerHeight = layer.size.height / MELFloatMax(layer.scrollRate.y, 1.0f);
        map.super.size = MELIntSizeMake(layerWidth > map.super.size.width ? layerWidth : map.super.size.width, layerHeight > map.super.size.height ? layerHeight : map.super.size.height);
    }

    return map;
}

void MELMmkProjectFormatWriteMap(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELMutableMap map) {
    if (self->version >= 7) {
        int32_t index = -1;
        for (unsigned int mapIndex = 0; mapIndex < project.root.maps.count; mapIndex++) {
            if (project.root.maps.memory[mapIndex].super.layers.memory == map.super.layers.memory) {
                index = mapIndex;
                break;
            }
        }
        MELOutputStreamWriteInt(outputStream, index);
    }

    if (self->version >= 6) {
        MELOutputStreamWriteNullableString(outputStream, map.name);
    }

    // Background color.
    self->class->writeColor(self, project, outputStream, MELColorToMELUInt8Color(map.super.backgroundColor));

    // Palette.
    MELMmkProjectFormatWritePaletteAsReference(self, project, outputStream, map.palette);

    // Layers.
    MELOutputStreamWriteInt(outputStream, (int32_t) map.super.layers.count);

    for (unsigned int index = 0; index < map.super.layers.count; index++) {
        self->class->writeLayer(self, project, outputStream, map.super.layers.memory[index]);
    }
}

MELSpriteDefinition MELMmkProjectFormatReadSpriteDefinition(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    char *name = NULL;
    int width, height;
    int type = 0;
    MELBoolean exportable = true;
    MELBoolean global = false;
    char *loadScript = NULL;
    char *scriptFile = NULL;
    
    if (self->version >= 4) {
        name = MELInputStreamReadNullableString(inputStream);
        width = MELInputStreamReadInt(inputStream);
        height = MELInputStreamReadInt(inputStream);
        type = MELInputStreamReadInt(inputStream);
        if (self->version >= 9) {
            /* distance */ MELInputStreamReadInt(inputStream);
        }
        if (self->version >= 10) {
            exportable = MELInputStreamReadBoolean(inputStream);
        }
        if (self->version >= 11) {
            global = MELInputStreamReadBoolean(inputStream);
        }
        loadScript = MELInputStreamReadNullableString(inputStream);
        scriptFile = MELInputStreamReadNullableString(inputStream);
    } else if (self->version == 4) {
        width = MELInputStreamReadInt(inputStream);
        height = MELInputStreamReadInt(inputStream);
    } else {
        width = MELInputStreamReadInt(inputStream);
        height = width;
    }

    MELPaletteRef defaultColorPalette = NULL;
    for (size_t index = 0; index < project->palettes.count; index++) {
        MELPaletteRef palette = project->palettes.memory[index];
        if (palette->name != NULL && !strcmp(palette->name, "Default color palette")) {
            defaultColorPalette = palette;
            break;
        }
    }

    MELSpriteDefinition spriteDefinition;
    spriteDefinition.name = name;
    spriteDefinition.type = type;
    spriteDefinition.palette = defaultColorPalette;
    spriteDefinition.animations = MELAnimationDefinitionListEmpty;
    spriteDefinition.motionName = scriptFile;
    spriteDefinition.loadScript = loadScript;

    MELAnimationDefinitionListEnsureCapacity(&spriteDefinition.animations, project->animationNames.count);
    memset(spriteDefinition.animations.memory, 0, sizeof(MELAnimationDefinition) * spriteDefinition.animations.capacity);
    spriteDefinition.animations.count = spriteDefinition.animations.capacity;

    const int animationCount = MELInputStreamReadInt(inputStream);
    for (int animation = 0; animation < animationCount; animation++) {
        MELAnimationDefinition animationDefinition = self->class->readAnimationDefinition(self, project, inputStream);
        int index = MELStringListIndexOf(project->animationNames, animationDefinition.name);
        if (index >= 0) {
            spriteDefinition.animations.memory[index] = animationDefinition;
        } else {
            fprintf(stderr, "Unable to load animation %d because name %s was not found in animationNames.\n", animation, animationDefinition.name);
        }
    }

    return spriteDefinition;
}

MELIntSize guessSpriteSize(MELSpriteDefinition spriteDefinition) {
    MELIntSize size = MELIntSizeZero;
    for (unsigned int index = 0; index < spriteDefinition.animations.count; index++) {
        MELAnimationDefinition animationDefinition = spriteDefinition.animations.memory[index];
        if (animationDefinition.frameCount > 0 && animationDefinition.images != NULL) {
            size = animationDefinition.images[0].size;
            if (MELStringEquals(animationDefinition.name, "stand")) {
                break;
            }
        }
    }
    return MELIntSizeEquals(size, MELIntSizeZero)
            ? MELIntSizeMake(32, 32)
            : size;
}

void MELMmkProjectFormatWriteSpriteDefinition(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELSpriteDefinition spriteDefinition) {
    // Name.
    MELOutputStreamWriteNullableString(outputStream, spriteDefinition.name);

    // General properties.
    MELIntSize spriteSize = guessSpriteSize(spriteDefinition);
    MELOutputStreamWriteInt(outputStream, spriteSize.width);
    MELOutputStreamWriteInt(outputStream, spriteSize.height);
    MELOutputStreamWriteInt(outputStream, spriteDefinition.type);
    
    if (self->version >= 9) {
        MELOutputStreamWriteInt(outputStream, /* distance */ 1);
    }
    if (self->version >= 10) {
        MELOutputStreamWriteBoolean(outputStream, /* exportable */ true);
    }
    if (self->version >= 11) {
        MELOutputStreamWriteBoolean(outputStream, /* global */ false);
    }

    // Script
    MELOutputStreamWriteNullableString(outputStream, spriteDefinition.loadScript);
    MELOutputStreamWriteNullableString(outputStream, spriteDefinition.motionName);
    
    MELOutputStreamWriteInt(outputStream, (int32_t) spriteDefinition.animations.count);
    for (unsigned int index = 0; index < spriteDefinition.animations.count; index++) {
        self->class->writeAnimationDefinition(self, project, outputStream, spriteDefinition.animations.memory[index]);
    }
}

MELAnimationDefinition MELMmkProjectFormatReadAnimationDefinition(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    MELAnimationDefinition animationDefinition;
    animationDefinition.name = MELInputStreamReadString(inputStream);
    animationDefinition.frequency = MELInputStreamReadInt(inputStream);

    MELAnimationType type = MELAnimationTypeNone;
    MELBoolean isLooping = false;
    MELBoolean isScrolling = false;
    if (self->version >= 4) {
        isLooping = MELInputStreamReadBoolean(inputStream);
    }
    if (self->version >= 9) {
        isScrolling = MELInputStreamReadBoolean(inputStream);
    }
    animationDefinition.type = type;
    animationDefinition.isScrolling = isScrolling;

    animationDefinition.frameCount = 0;
    animationDefinition.frames = NULL;
    animationDefinition.images = NULL;

    const int directionCount = MELInputStreamReadInt(inputStream);
    for (int i = 0; i < directionCount; i++) {
        const double direction = MELInputStreamReadDouble(inputStream);
        const int frameCount = MELInputStreamReadInt(inputStream);
        if (direction == 0) {
            animationDefinition.frameCount = frameCount;
            animationDefinition.images = frameCount > 0 ? malloc(sizeof(MELImagePaletteImage) * frameCount) : NULL;
            animationDefinition.type = MELAnimationTypeForFrameCountAndLooping(frameCount, isLooping);
        }

        for (int frameIndex = 0; frameIndex < frameCount; frameIndex++) {
            MELImagePaletteImage frame = self->class->readImagePaletteImage(self, project, inputStream);
            MELHitboxDecorator hitboxDecorator = (MELHitboxDecorator){{MELDecoratorTypeHitbox}, MELIntRectangleZero};

            if (self->version >= 8) {
                hitboxDecorator.hitbox = self->class->readRectangle(self, project, inputStream);
            }

            if (direction == 0) {
                if (self->version >= 8) {
                    MELHitboxDecorator *hitboxDecoratorRef = malloc(sizeof(MELHitboxDecorator));
                    *hitboxDecoratorRef = hitboxDecorator;
                    MELDecoratorRefListPush(&frame.decorators, &hitboxDecoratorRef->super);
                }
                animationDefinition.images[frameIndex] = frame;
            } else {
                MELImagePaletteImageDeinit(&frame);
            }
        }
    }

    return animationDefinition;
}

void MELMmkProjectFormatWriteAnimationDefinition(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELAnimationDefinition animationDefinition) {
    MELOutputStreamWriteString(outputStream, animationDefinition.name != NULL ? animationDefinition.name : "");
    MELOutputStreamWriteInt(outputStream, animationDefinition.frequency);
    if (self->version >= 4) {
        MELOutputStreamWriteBoolean(outputStream, animationDefinition.type == MELAnimationTypeLooping || animationDefinition.type == MELAnimationTypeSynchronized);
    }
    if (self->version >= 9) {
        MELOutputStreamWriteBoolean(outputStream, animationDefinition.isScrolling);
    }

    if (animationDefinition.images == NULL || animationDefinition.frameCount == 0) {
        MELOutputStreamWriteInt(outputStream, /** directions.count */ 0);
        return;
    }

    MELOutputStreamWriteInt(outputStream, /** directions.count */ 1);
    MELOutputStreamWriteDouble(outputStream, /* direction */ 0.0);

    MELOutputStreamWriteInt(outputStream, animationDefinition.frameCount);
    for (unsigned int index = 0; index < animationDefinition.frameCount; index++) {
        MELImagePaletteImage image = animationDefinition.images[index];
        self->class->writeImagePaletteImage(self, project, outputStream, image);

        if (self->version >= 8) {
            MELHitboxDecorator *hitboxPlugin = (MELHitboxDecorator *) MELDecoratorRefListForType(image.decorators, MELDecoratorTypeHitbox);
            if (hitboxPlugin != NULL) {
                self->class->writeRectangle(self, project, outputStream, hitboxPlugin->hitbox);
            } else {
                MELOutputStreamWriteBoolean(outputStream, false);
            }
        }
    }
}

MELSpriteInstance MELMmkProjectFormatReadSpriteInstance(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    const int x = MELInputStreamReadInt(inputStream);
    const int y = MELInputStreamReadInt(inputStream);
    const int index = MELInputStreamReadInt(inputStream);
    MELBoolean unique;
    char *script;

    if (self->version >= 4) {
        unique = MELInputStreamReadBoolean(inputStream);
        script = MELInputStreamReadNullableString(inputStream);
    } else {
        unique = false;
        script = NULL;
    }

    return MELSpriteInstanceMake(index, MELPointMake((GLfloat)x, (GLfloat)y), unique, script);
}

void MELMmkProjectFormatWriteSpriteInstance(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELSpriteInstance spriteInstance) {
    MELOutputStreamWriteInt(outputStream, spriteInstance.topLeft.x);
    MELOutputStreamWriteInt(outputStream, spriteInstance.topLeft.y);
    MELOutputStreamWriteInt(outputStream, spriteInstance.definitionIndex);
    
    if (self->version >= 4) {
        MELOutputStreamWriteBoolean(outputStream, spriteInstance.isUnique);
        MELOutputStreamWriteNullableString(outputStream, spriteInstance.initializationScript);
    }
}

const MELProjectFormatClass MELMmkProjectFormatClass = {
    .openProject = &MELMmkProjectFormatOpenProject,
    .openProjectAtPath = &MELMmkProjectFormatOpenProjectAtPath,
    .saveProject = &MELMmkProjectFormatSaveProject,
    .saveProjectAtPath = &MELMmkProjectFormatSaveProjectAtPath,

    .readRectangle = &MELMmkProjectFormatReadRectangle,
    .writeRectangle = &MELMmkProjectFormatWriteRectangle,
    .readColor = &MELMmkProjectFormatReadColor,
    .writeColor = &MELMmkProjectFormatWriteColor,
    .readPalette = &MELMmkProjectFormatReadPalette,
    .writePalette = &MELMmkProjectFormatWritePalette,
    .readColorPalette = &MELMmkProjectFormatReadColorPalette,
    .writeColorPalette = &MELMmkProjectFormatWriteColorPalette,
    .readImagePalette = &MELMmkProjectFormatReadImagePalette,
    .writeImagePalette = &MELMmkProjectFormatWriteImagePalette,
    .readImagePaletteImage = &MELMmkProjectFormatReadImagePaletteImage,
    .writeImagePaletteImage = &MELMmkProjectFormatWriteImagePaletteImage,
    .readPaletteReference = &MELMmkProjectFormatReadPaletteReference,
    .writePaletteReference = &MELMmkProjectFormatWritePaletteReference,
    .readScrollRate = &MELMmkProjectFormatReadScrollRate,
    .writeScrollRate = &MELMmkProjectFormatWriteScrollRate,
    .readLayer = &MELMmkProjectFormatReadLayer,
    .writeLayer = &MELMmkProjectFormatWriteLayer,
    .readMap = &MELMmkProjectFormatReadMap,
    .writeMap = &MELMmkProjectFormatWriteMap,
    .readSpriteDefinition = &MELMmkProjectFormatReadSpriteDefinition,
    .writeSpriteDefinition = &MELMmkProjectFormatWriteSpriteDefinition,
    .readAnimationDefinition = &MELMmkProjectFormatReadAnimationDefinition,
    .writeAnimationDefinition = &MELMmkProjectFormatWriteAnimationDefinition,
    .readSpriteInstance = &MELMmkProjectFormatReadSpriteInstance,
    .writeSpriteInstance = &MELMmkProjectFormatWriteSpriteInstance
};
