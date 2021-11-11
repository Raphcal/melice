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
    mz_uint dataIndex;
    if (mz_zip_reader_locate_file_v2(&archive, DATA_ENTRY, NULL, 0, &dataIndex) != MZ_TRUE) {
        fprintf(stderr, "Could not find data entry in MMK file at path: %s\n", path);
        return false;
    }
    mz_zip_reader_end(&archive);

    MELProject project = {MELPaletteListEmpty, MELMapGroupListEmpty};

    *outProject = project;
    return true;
}

const MELProjectFormatClass MELMmkProjectFormatClass = {
    .openProject = &MELMmkProjectFormatOpenProject
};
