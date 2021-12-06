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

#define LAST_SUPPORTED_VERSION 11
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

    MELProject project = {MELPaletteRefListEmpty, MELMapGroupListEmpty};

    MELMapGroupListPush(&project.mapGroups, MELMapGroupEmpty);

    // Loading palettes.
    const int paletteCount = MELInputStreamReadInt(inputStream);
    for (int index = 0; index < paletteCount; index++) {
        MELPaletteRefListPush(&project.palettes, self->class->readPalette(self, &project, inputStream));
    }

    // Loading maps.
    const int mapCount = MELInputStreamReadInt(inputStream);
    for (int index = 0; index < mapCount; index++) {
        MELMutableMapListPush(&project.mapGroups.memory[0].maps, self->class->readMap(self, &project, inputStream));
    }

    if (version >= 3) {
        // Loading sprites.
        const int spriteCount = MELInputStreamReadInt(inputStream);
        for (int index = 0; index < spriteCount; index++) {
            MELSpriteDefinitionListPush(&project.mapGroups.memory[0].sprites, self->class->readSpriteDefinition(self, &project, inputStream));
        }

        // Loading sprite instances.
        const MELPoint defaultScrollRate = MELPointMake(1, 1);
        for (int mapIndex = 0; mapIndex < mapCount; mapIndex++) {
            MELLayerList * _Nonnull layers = &project.mapGroups.memory[0].maps.memory[mapIndex].layers;
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
    MELMutableMapList maps = project.mapGroups.count == 1 ? project.mapGroups.memory[0].maps : MELMutableMapListEmpty;
    MELOutputStreamWriteInt(outputStream, (int32_t) maps.count);
    for (unsigned int index = 0; index < maps.count; index++) {
        self->class->writeMap(self, project, outputStream, maps.memory[index]);
    }

    if (self->version >= 3) {
        // Writing sprites definitions.
        MELSpriteDefinitionList sprites = project.mapGroups.count == 1 ? project.mapGroups.memory[0].sprites : MELSpriteDefinitionListEmpty;
        MELOutputStreamWriteInt(outputStream, (int32_t) sprites.count);
        for (unsigned int index = 0; index < sprites.count; index++) {
            self->class->writeSpriteDefinition(self, project, outputStream, sprites.memory[index]);
        }
        
        // Writing sprites instances.
        for (unsigned int mapIndex = 0; mapIndex < maps.count; mapIndex++) {
            const MELMutableMap map = maps.memory[mapIndex];
            MELSpriteInstanceList instances = MELSpriteInstanceListEmpty;
            for (unsigned int layerIndex = 0; layerIndex < map.layers.count; layerIndex++) {
                MELSpriteInstanceListAddAll(&instances, map.layers.memory[layerIndex].sprites);
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
    int32_t tileSize = MELInputStreamReadInt(inputStream);
    MELIntSize size = MELIntSizeMake(MELInputStreamReadInt(inputStream), MELInputStreamReadInt(inputStream));
    MELIntSize tileCount = MELIntSizeMake(size.width / tileSize, size.height / tileSize);
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
    imagePalette->super.tileSize = MELIntSizeMake(tileSize, tileSize);
    imagePalette->super.count = imageCount;
    imagePalette->images = malloc(sizeof(MELImagePaletteImage) * imageCount);
    imagePalette->colorPalette = colorPalette;

    const int imageTileCount = tileSize * tileSize;
    for (unsigned int imageIndex = 0; imageIndex < imageCount; imageIndex++) {
        MELImagePaletteImage image = (MELImagePaletteImage) {malloc(sizeof(int) * imageTileCount), MELDecoratorRefListEmpty};
        const int imageX = imageIndex / tileCount.width;
        const int imageY = imageIndex % tileCount.width;
        int *row = atlas + imageX * tileSize + imageY * tileSize * tileCount.width;
        for (unsigned int y = 0; y < tileSize; y++) {
            memcpy(image.tiles + y * tileSize, row, sizeof(int) * tileSize);
            row += tileSize * tileCount.width;
        }
        imagePalette->images[imageIndex] = image;
    }

    return &imagePalette->super;
}

MELPaletteRef MELMmkProjectFormatReadPalette(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    char *paletteClass = MELInputStreamReadString(inputStream);
    if (strcmp(paletteClass, "fr.rca.mapmaker.model.palette.AlphaColorPalette") == 0) {
        return &self->class->readColorPalette(self, project, inputStream)->super;
    } else if (strcmp(paletteClass, "fr.rca.mapmaker.model.palette.EditableImagePalette") == 0) {
        return &self->class->readImagePalette(self, project, inputStream)->super;
    } else if (strcmp(paletteClass, "fr.rca.mapmaker.model.palette.PaletteReference") == 0) {
        return self->class->readPaletteReference(self, project, inputStream);
    } else if (strcmp(paletteClass, "fr.rca.mapmaker.model.palette.ImagePalette") == 0) {
        return MELMmkProjectFormatReadBufferedImagePalette(inputStream);
    } else {
        fprintf(stderr, "Unsupported palette class: %s\n", paletteClass);
        return NULL;
    }
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
    MELMmkProjectFormatWritePalette(self, project, outputStream, palette);
}

MELColorPalette * _Nullable MELMmkProjectFormatReadColorPalette(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    const int count = MELInputStreamReadInt(inputStream);

    MELColorPalette colorPalette = {{&MELColorPaletteClass, NULL, MELIntSizeMake(1, 1), count}, malloc(sizeof(MELUInt8RGBColor) * count), malloc(sizeof(uint8_t) * 8), 8};
    uint8_t defaultAlphaLevels[] = {255, 224, 192, 160, 128, 96, 64, 32};
    memcpy(colorPalette.alphaLevels, defaultAlphaLevels, sizeof(uint8_t) * 8);

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
    /* columns */ MELInputStreamReadInt(inputStream);

    MELColorPalette *colorPalette = self->class->readColorPalette(self, project, inputStream);

    const int count = MELInputStreamReadInt(inputStream);

    MELImagePalette imagePalette = {{&MELImagePaletteClass, name, MELIntSizeMake(tileSize, tileSize), count}, colorPalette, malloc(sizeof(MELImagePaletteImage) * count)};

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
    MELOutputStreamWriteInt(outputStream, /* columns */ 4);

    self->class->writeColorPalette(self, project, outputStream, imagePalette->colorPalette);

    const uint32_t count = imagePalette->super.count;
    MELOutputStreamWriteInt(outputStream, count);

    const MELIntSize tileSize = imagePalette->super.tileSize;
    const MELLayer baseLayer = (MELLayer) {NULL, tileSize, tileSize.width * tileSize.height, NULL, MELPointZero, MELSpriteInstanceListEmpty};

    MELImagePaletteImage *images = imagePalette->images;
    for (unsigned int index = 0; index < count; index++) {
        MELImagePaletteImage image = images[index];
        MELLayer layer = baseLayer;
        layer.tiles = image.tiles;
        self->class->writeLayer(self, project, outputStream, layer);

        if (self->version >= 3) {
            char *function = NULL;
            for (unsigned int decoratorIndex = 0; decoratorIndex < image.decorators.count; index++) {
                if (image.decorators.memory[index]->type == MELDecoratorTypeFunction) {
                    MELFunctionDecorator *functionDecorator = (MELFunctionDecorator *) image.decorators.memory[index];
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
    return (MELImagePaletteImage) {layer.tiles, MELDecoratorRefListEmpty};
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

    int32_t count;
    int32_t *tiles = MELInputStreamReadIntArray(inputStream, &count);

    return (MELLayer) {name, MELIntSizeMake(width, height), count, tiles, scrollRate, MELSpriteInstanceListEmpty};
}

void MELMmkProjectFormatWriteLayer(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELLayer layer) {
    MELOutputStreamWriteString(outputStream, layer.name != NULL ? layer.name : "");
    MELOutputStreamWriteInt(outputStream, layer.size.width);
    MELOutputStreamWriteInt(outputStream, layer.size.height);
    self->class->writeScrollRate(self, project, outputStream, layer.scrollRate);
    MELOutputStreamWriteIntArray(outputStream, layer.tiles, layer.size.width * layer.size.height);
}

MELMutableMap MELMmkProjectFormatReadMap(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    MELMutableMap map;
    if (self->version >= 7) {
        /* index */ MELInputStreamReadInt(inputStream);
    }
    if (self->version >= 6) {
        map.name = MELInputStreamReadNullableString(inputStream);
    }
    map.backgroundColor = MELColorMakeWithMELUInt8Color(self->class->readColor(self, project, inputStream));
    map.palette = self->class->readPalette(self, project, inputStream);

    map.size = MELIntSizeZero;

    const int layerCount = MELInputStreamReadInt(inputStream);
    map.layers = MELLayerListMakeWithInitialCapacity(layerCount);
    for (int index = 0; index < layerCount; index++) {
        MELLayer layer = self->class->readLayer(self, project, inputStream);
        MELLayerListPush(&map.layers, layer);

        int32_t layerWidth = layer.size.width / MELFloatMax(layer.scrollRate.x, 1.0f);
        int32_t layerHeight = layer.size.height / MELFloatMax(layer.scrollRate.y, 1.0f);
        map.size = MELIntSizeMake(layerWidth > map.size.width ? layerWidth : map.size.width, layerHeight > map.size.height ? layerHeight : map.size.height);
    }

    return map;
}

void MELMmkProjectFormatWriteMap(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELMutableMap map) {
    if (self->version >= 7) {
        int32_t index = -1;
        MELMutableMapList maps = project.mapGroups.count == 1 ? project.mapGroups.memory[0].maps : MELMutableMapListEmpty;
        for (unsigned int mapIndex = 0; index < maps.count; mapIndex++) {
            if (maps.memory[mapIndex].layers.memory == map.layers.memory) {
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
    self->class->writeColor(self, project, outputStream, MELColorToMELUInt8Color(map.backgroundColor));
    
    // Palette.
    MELMmkProjectFormatWritePaletteAsReference(self, project, outputStream, map.palette);

    // Layers.
    MELOutputStreamWriteInt(outputStream, (int32_t) map.layers.count);
    
    for (unsigned int index = 0; index < map.layers.count; index++) {
        self->class->writeLayer(self, project, outputStream, map.layers.memory[index]);
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

    MELSpriteDefinition spriteDefinition = {name, type, MELAnimationDefinitionListEmpty, scriptFile, loadScript};

    const int animationCount = MELInputStreamReadInt(inputStream);
    MELAnimationDefinitionListEnsureCapacity(&spriteDefinition.animations, animationCount);
    for (int animation = 0; animation < animationCount; animation++) {
        MELAnimationDefinitionListPush(&spriteDefinition.animations, self->class->readAnimationDefinition(self, project, inputStream));
    }

    return spriteDefinition;
}

void MELMmkProjectFormatWriteSpriteDefinition(MELProjectFormat * _Nonnull self, MELProject project, MELOutputStream * _Nonnull outputStream, MELSpriteDefinition spriteDefinition) {
    // Name.
    MELOutputStreamWriteNullableString(outputStream, spriteDefinition.name);

    // General properties.
    MELOutputStreamWriteInt(outputStream, /* width */ 32);
    MELOutputStreamWriteInt(outputStream, /* height */ 32);
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

    MELAnimationType type = MELAnimationTypePlayOnce;
    MELBoolean isLooping = false;
    MELBoolean isScrolling = false;
    if (self->version >= 4) {
        isLooping = MELInputStreamReadBoolean(inputStream);
        type = MELAnimationTypeLooping;
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
        }

        MELImagePaletteImageList frames = MELImagePaletteImageListMakeWithInitialCapacity(frameCount);
        for (int frame = 0; frame < frameCount; frame++) {
            MELLayer layer = self->class->readLayer(self, project, inputStream);
            free(layer.name);
            MELImagePaletteImage frame = (MELImagePaletteImage) {layer.tiles, MELDecoratorRefListMakeWithInitialCapacity(1)};
            MELSizeDecorator *size = malloc(sizeof(MELSizeDecorator));
            size->super.type = MELDecoratorTypeSize;
            size->size = layer.size;
            MELDecoratorRefListPush(&frame.decorators, (MELDecoratorRef) size);

            if (self->version >= 8) {
                MELHitboxDecorator *hitboxDecorator = malloc(sizeof(MELHitboxDecorator));
                hitboxDecorator->super.type = MELDecoratorTypeHitbox;
                hitboxDecorator->hitbox = self->class->readRectangle(self, project, inputStream);
                MELDecoratorRefListPush(&frame.decorators, &hitboxDecorator->super);
            }

            MELImagePaletteImageListPush(&frames, frame);
        }

        animationDefinition.images = frames.memory;
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
        MELSizeDecorator *sizeDecorator = MELDecoratorRefListGetSizeDecorator(image.decorators);
        MELIntSize size = sizeDecorator != NULL ? sizeDecorator->size : MELIntSizeZero;
        MELLayer layer = (MELLayer) {NULL, size, size.width * size.height, image.tiles, MELPointZero, MELSpriteInstanceListEmpty};
        self->class->writeLayer(self, project, outputStream, layer);

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
    .writeImagePaletteImage = NULL,
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
