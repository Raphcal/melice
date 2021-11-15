//
//  mmkprojectformat.c
//  melice
//
//  Created by Raphaël Calabro on 07/11/2021.
//

#include "mmkprojectformat.h"

#include "../dependencies/miniz/miniz.h"

#define DATA_ENTRY "data"

MELBoolean MELMmkProjectFormatOpenProject(MELProjectFormat * _Nonnull self, const char * _Nonnull path, MELProject * _Nonnull outProject) {
    mz_zip_archive archive;
    mz_zip_reader_init_file(&archive, path, 0);
    size_t size;
    void * _Nullable data = mz_zip_reader_extract_file_to_heap(&archive, DATA_ENTRY, &size, 0);
    mz_zip_reader_end(&archive);
    if (data == NULL) {
        fprintf(stderr, "Unable to extract data of MMK file at path: %s\n", path);
        return false;
    }
    MELInputStream inputStream = MELInputStreamInitWithBytes(data, size);
    mz_free(data);

    int8_t header[4];
    MELInputStreamRead(&inputStream, header, 4);

    unsigned int version;
    if (memcmp(header, "MMK", 3) == 0) {
        version = header[3] - '0';
    } else if (memcmp(header, "MM", 2) == 0) {
        version = (header[2] - '0') * 10 + header[3] - '0';
    } else {
        version = 1;
        inputStream.cursor = 0;
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
                if (MELPointEquals(layers->memory[layerIndex].scrollRate, defaultScrollRate)) {
                    layer = layers->memory + layerIndex;
                    break;
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

    MELInputStreamDeinit(&inputStream);

    *outProject = project;
    return true;
}

MELPaletteRef MELMmkProjectFormatReadPalette(MELProjectFormat * _Nonnull self, MELProject * _Nonnull project, MELInputStream * _Nonnull inputStream) {
    char *paletteClass = MELInputStreamReadString(inputStream);
    // TODO: Convert UTF-16 to UTF-8
    if (strcmp(paletteClass, "fr.rca.mapmaker.model.palette.AlphaColorPalette") == 0) {
        return &self->class->readColorPalette(self, project, inputStream)->super;
    } else if (strcmp(paletteClass, "fr.rca.mapmaker.model.palette.EditableImagePalette") == 0) {
        return &self->class->readImagePalette(self, project, inputStream)->super;
    } else {
        fprintf(stderr, "Unsupported palette class: %s\n", paletteClass);
        return NULL;
    }
}

const MELProjectFormatClass MELMmkProjectFormatClass = {
    .openProject = &MELMmkProjectFormatOpenProject,
    .readPalette = &MELMmkProjectFormatReadPalette
};
