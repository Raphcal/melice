//
//  filemanager.c
//  shmup
//
//  Created by Raphaël Calabro on 31/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "filemanager.h"

#include <string.h>
#include <sys/stat.h>

#ifndef _WIN32
#include <libgen.h>
#include <sys/param.h>
#endif


#define RESOURCE_DIRECTORY MELPathSeparatorString ".." MELPathSeparatorString "Resources"
#define RESOURCE_DIRECTORY_LENGTH 13
#define ASSETS_DIRECTORY MELPathSeparatorString "assets"
#define ASSETS_DIRECTORY_LENGTH 7

MELFileManager sharedFileManager = {NULL, 0};

MELBoolean MELFileManagerIsDirectory(const char * _Nonnull path);

MELFileManager * _Nonnull MELFileManagerGetSharedInstance(void) {
    return &sharedFileManager;
}

#ifdef _WIN32
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
char buffer[1024];
char *dirname(const char *path) {
	_splitpath_s(path, NULL, 0, buffer, 1024, NULL, 0, NULL, 0);
	return buffer;
}
#endif

void MELFileManagerInitWithArguments(MELFileManager * _Nonnull self, char * _Nonnull argv[_Nonnull]) {
    const char *applicationDirectory = dirname((char *)argv[0]);
    const size_t applicationDirectoryLength = strlen(applicationDirectory);
    
    char *assetsRoot;
    size_t assetsRootLength;

#ifdef __APPLE__
    char realAppDirectory[PATH_MAX];
    realpath(applicationDirectory, realAppDirectory);

    assetsRootLength = applicationDirectoryLength + RESOURCE_DIRECTORY_LENGTH;
    assetsRoot = calloc(assetsRootLength + 1, sizeof(char));
    strcpy(assetsRoot, applicationDirectory);
    strcat(assetsRoot, RESOURCE_DIRECTORY);
    
    const MELBoolean isMacOSBundle = strcmp(basename(realAppDirectory), "MacOS") == 0 && MELFileManagerIsDirectory(assetsRoot);
    
    if (!isMacOSBundle) {
        free(assetsRoot);
        
        assetsRootLength = applicationDirectoryLength + ASSETS_DIRECTORY_LENGTH;
        assetsRoot = calloc(assetsRootLength + 1, sizeof(char));
        strcpy(assetsRoot, applicationDirectory);
        strcat(assetsRoot, ASSETS_DIRECTORY);
    }
#else
    assetsRootLength = applicationDirectoryLength + ASSETS_DIRECTORY_LENGTH;
    assetsRoot = calloc(assetsRootLength + 1, sizeof(char));
    strcpy(assetsRoot, applicationDirectory);
    strcat(assetsRoot, ASSETS_DIRECTORY);
#endif
    self->assetsRoot = assetsRoot;
    self->assetsRootLength = assetsRootLength;
}

void MELFileManagerInitWithAssetsRoot(MELFileManager * _Nonnull self, const char * _Nonnull assetsRoot) {
    const size_t assetsRootLength = strlen(assetsRoot);
    self->assetsRoot = calloc(assetsRootLength + 1, sizeof(char));
    memcpy(self->assetsRoot, assetsRoot, assetsRootLength);
    self->assetsRootLength = assetsRootLength;
}

void MELFileManagerDeinit(MELFileManager * _Nonnull self) {
    free(self->assetsRoot);
    self->assetsRoot = NULL;
    self->assetsRootLength = 0;
}

char * _Nonnull MELFileManagerPathForAsset(MELFileManager * _Nonnull self, const char * _Nonnull fileName) {
    char *filePath = calloc(self->assetsRootLength + strlen(fileName) + 2, sizeof(char));
    strcpy(filePath, self->assetsRoot);
    filePath[self->assetsRootLength] = MELPathSeparator;
    strcat(filePath, fileName);
    return filePath;
}

MELInputStream MELFileManagerOpenAsset(MELFileManager * _Nonnull self, const char * _Nonnull fileName) {
    char *filePath = MELFileManagerPathForAsset(self, fileName);
    MELInputStream inputStream = MELInputStreamOpen(filePath);
    free(filePath);
    return inputStream;
}

MELOutputStream MELFileManagerWriteAsset(MELFileManager * _Nonnull self, const char * _Nonnull fileName) {
    char *filePath = MELFileManagerPathForAsset(self, fileName);
    MELOutputStream outputStream = MELOutputStreamOpen(filePath);
    free(filePath);
    return outputStream;
}

MELBoolean MELFileManagerIsDirectory(const char * _Nonnull path) {
    struct stat pathStat;
    return stat(path, &pathStat) == 0 && S_ISDIR(pathStat.st_mode);
}
