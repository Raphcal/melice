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

MELBoolean MELMmkProjectFormatOpenProject(MELProjectFormat * _Nonnull self, const char * _Nonnull path, MELProject * _Nonnull outProject) {
    MELInputStream inputStream;
    if (!openFileOrArchiveAtPath(path, &inputStream)) {
        return false;
    }

    uint16_t header[5];
    MELInputStreamRead(&inputStream, header, sizeof(uint16_t) * 4);
    header[4] = 0;
    char *utf8Header = MELUTF8StringMakeWithUTF16String(header);

    unsigned int version;
    if (memcmp(utf8Header, "MMK", 3) == 0) {
        version = header[3] - '0';
    } else if (memcmp(utf8Header, "MM", 2) == 0) {
        version = (header[2] - '0') * 10 + header[3] - '0';
    } else {
        version = 1;
        inputStream.cursor = 0;
    }
    free(utf8Header);

    if (version > LAST_SUPPORTED_VERSION) {
        fprintf(stderr, "Project version %d is unsupported, last supported version is %d\n", version, LAST_SUPPORTED_VERSION);
        MELInputStreamClose(&inputStream);
        return false;
    }

    self->version = version;

    MELProject project = {MELPaletteRefListEmpty, MELMapGroupListEmpty};

    MELMapGroupListPush(&project.mapGroups, MELMapGroupEmpty);

    // Loading palettes.
    const int paletteCount = MELInputStreamReadInt(&inputStream);
    for (int index = 0; index < paletteCount; index++) {
        MELPaletteRefListPush(&project.palettes, self->class->readPalette(self, &project, &inputStream));
    }

    // Loading maps.
    const int mapCount = MELInputStreamReadInt(&inputStream);
    for (int index = 0; index < mapCount; index++) {
        MELMutableMapListPush(&project.mapGroups.memory[0].maps, self->class->readMap(self, &project, &inputStream));
    }

    if (version >= 3) {
        // Loading sprites.
        const int spriteCount = MELInputStreamReadInt(&inputStream);
        for (int index = 0; index < spriteCount; index++) {
            MELSpriteDefinitionListPush(&project.mapGroups.memory[0].sprites, self->class->readSpriteDefinition(self, &project, &inputStream));
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

            const int instanceCount = MELInputStreamReadInt(&inputStream);
            for (int instanceIndex = 0; instanceIndex < instanceCount; instanceIndex++) {
                MELSpriteInstance instance = self->class->readSpriteInstance(self, &project, &inputStream);
                if (layer != NULL) {
                    MELSpriteInstanceListPush(&layer->sprites, instance);
                }
            }
        }
    }

    MELInputStreamClose(&inputStream);

    *outProject = project;
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

MELUInt8Color MELMmkProjectFormatReadColor(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    const int red = MELInputStreamReadInt(inputStream);
    const int green = MELInputStreamReadInt(inputStream);
    const int blue = MELInputStreamReadInt(inputStream);
    const int alpha = MELInputStreamReadInt(inputStream);
    return MELUInt8ColorMake(red, green, blue, alpha);
}

MELPaletteRef MELMmkProjectFormatReadPalette(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    char *paletteClass = MELInputStreamReadString(inputStream);
    if (strcmp(paletteClass, "fr.rca.mapmaker.model.palette.AlphaColorPalette") == 0) {
        return &self->class->readColorPalette(self, project, inputStream)->super;
    } else if (strcmp(paletteClass, "fr.rca.mapmaker.model.palette.EditableImagePalette") == 0) {
        return &self->class->readImagePalette(self, project, inputStream)->super;
    } else if (strcmp(paletteClass, "fr.rca.mapmaker.model.palette.PaletteReference") == 0) {
        return self->class->readPaletteReference(self, project, inputStream);
    } else {
        fprintf(stderr, "Unsupported palette class: %s\n", paletteClass);
        return NULL;
    }
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

MELImagePaletteImage MELMmkProjectFormatReadImagePaletteImage(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    MELLayer layer = self->class->readLayer(self, project, inputStream);
    free(layer.name);
    return (MELImagePaletteImage) {layer.tiles, MELDecoratorRefListEmpty};
}

MELPaletteRef MELMmkProjectFormatReadPaletteReference(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    const int paletteIndex = MELInputStreamReadInt(inputStream);
    return project->palettes.memory[paletteIndex];
}

MELPoint MELMmkProjectFormatReadScrollRate(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    const float x = MELInputStreamReadFloat(inputStream);
    const float y = self->version < 5 ? x : MELInputStreamReadFloat(inputStream);
    return MELPointMake(x, y);
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

    MELSpriteDefinition spriteDefinition = {name, type, MELAnimationDefinitionListEmpty, scriptFile};

    const int animationCount = MELInputStreamReadInt(inputStream);
    MELAnimationDefinitionListEnsureCapacity(&spriteDefinition.animations, animationCount);
    for (int animation = 0; animation < animationCount; animation++) {
        MELAnimationDefinitionListPush(&spriteDefinition.animations, self->class->readAnimationDefinition(self, project, inputStream));
    }

    return spriteDefinition;
}

MELAnimationDefinition MELMmkProjectFormatReadAnimationDefinition(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    MELAnimationDefinition animationDefinition;
    animationDefinition.name = MELInputStreamReadString(inputStream);
    animationDefinition.frequency = MELInputStreamReadInt(inputStream);

    MELAnimationType type = MELAnimationTypePlayOnce;
    MELBoolean isLooping = false;
    MELBoolean isScrolling = false;
    if (self->version >= 4) {
        // TODO: Add a property in MELAnimationDefinition to store isLooping value.
        isLooping = MELInputStreamReadBoolean(inputStream);
        type = MELAnimationTypeLooping;
    }
    if (self->version >= 9) {
        // TODO: Add a property in MELAnimationDefinition to store isScrolling value.
        isScrolling = MELInputStreamReadBoolean(inputStream);
    }
    animationDefinition.type = type;

    animationDefinition.frameCount = 0;

    const int directionCount = MELInputStreamReadInt(inputStream);
    for (int i = 0; i < directionCount; i++) {
        const double direction = MELInputStreamReadDouble(inputStream);
        const int frameCount = MELInputStreamReadInt(inputStream);
        if (direction == 0) {
            animationDefinition.frameCount = frameCount;
        }

        MELImagePaletteImageList frames = MELImagePaletteImageListMakeWithInitialCapacity(frameCount);
        for (int frame = 0; frame < frameCount; frame++) {
            MELImagePaletteImage frame = self->class->readImagePaletteImage(self, project, inputStream);

            if (self->version >= 8) {
                MELHitboxDecorator *hitboxDecorator = malloc(sizeof(MELHitboxDecorator));
                hitboxDecorator->super.type = MELDecoratorTypeHitbox;
                hitboxDecorator->hitbox = self->class->readRectangle(self, project, inputStream);
                MELDecoratorRefListPush(&frame.decorators, &hitboxDecorator->super);
            }

            MELImagePaletteImageListPush(&frames, frame);
        }

        // TODO: Add a property in MELAnimationDefinition to store raw frames.
        // animationDefinition.frames = frames;
    }

    return animationDefinition;
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

const MELProjectFormatClass MELMmkProjectFormatClass = {
    .openProject = &MELMmkProjectFormatOpenProject,

    .readRectangle = &MELMmkProjectFormatReadRectangle,
    .readColor = &MELMmkProjectFormatReadColor,
    .readPalette = &MELMmkProjectFormatReadPalette,
    .readColorPalette = &MELMmkProjectFormatReadColorPalette,
    .readImagePalette = &MELMmkProjectFormatReadImagePalette,
    .readImagePaletteImage = &MELMmkProjectFormatReadImagePaletteImage,
    .readPaletteReference = &MELMmkProjectFormatReadPaletteReference,
    .readScrollRate = &MELMmkProjectFormatReadScrollRate,
    .readLayer = &MELMmkProjectFormatReadLayer,
    .readMap = &MELMmkProjectFormatReadMap,
    .readSpriteDefinition = &MELMmkProjectFormatReadSpriteDefinition,
    .readAnimationDefinition = &MELMmkProjectFormatReadAnimationDefinition,
    .readSpriteInstance = &MELMmkProjectFormatReadSpriteInstance
};
