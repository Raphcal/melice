//
//  filemanager.h
//  shmup
//
//  Created by Raphaël Calabro on 31/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef filemanager_h
#define filemanager_h

#include "../melstd.h"

#include "inputstream.h"

typedef struct {
    char * _Nullable assetsRoot;
    size_t assetsRootLength;
} MELFileManager;

MELFileManager * _Nonnull MELFileManagerGetSharedInstance(void);

/**
 * Initialize the given file manager.
 *
 * @param self File Manager instance to initialize.
 * @param argv Array of arguments passed to the application. It should at least contain 1 argument (the application path).
 */
void MELFileManagerInitWithArguments(MELFileManager * _Nonnull self, char * _Nonnull argv[_Nonnull]);

/**
 * Deinitialize and free the resources used by the given file manager.
 * The file manager itself is not freed.
 *
 * @param self File Manager instance.
 */
void MELFileManagerDeinit(MELFileManager * _Nonnull self);

/**
 * Returns the path to the given asset.
 *
 * @param self File manager instance.
 * @param fileName Filename of the asset to open.
 * @return The path to the given asset. You are responsible for freeing the returned path.
 */
char * _Nonnull MELFileManagerPathForAsset(MELFileManager * _Nonnull self, const char * _Nonnull fileName);

/**
 * Open an input stream on the given asset.
 * If an error occured, the file property of the input stream will be NULL.
 *
 * @param self File manager instance.
 * @param fileName Filename of the asset to open.
 * @return An input stream.
 */
MELInputStream MELFileManagerOpenAsset(MELFileManager * _Nonnull self, const char * _Nonnull fileName);

#endif /* filemanager_h */
