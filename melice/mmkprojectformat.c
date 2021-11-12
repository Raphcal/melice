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

    if (memcmp(header, "MMK", 3) == 0) {
        self->version = header[3] - '0';
    } else if (memcmp(header, "MM", 2) == 0) {
        self->version = (header[2] - '0') * 10 + header[3] - '0';
    } else {
        self->version = 1;
        inputStream.cursor = 0;
    }

    MELProject project = {MELPaletteRefListEmpty, MELMapGroupListEmpty};

    int paletteCount = MELInputStreamReadInt(&inputStream);
    for(int index = 0; index < paletteCount; index++) {
        MELPaletteRefListPush(&project.palettes, self->class->readPalette(self, &project, &inputStream));
    }

    MELInputStreamDeinit(&inputStream);

    *outProject = project;
    return true;
}

const MELProjectFormatClass MELMmkProjectFormatClass = {
    .openProject = &MELMmkProjectFormatOpenProject
};
