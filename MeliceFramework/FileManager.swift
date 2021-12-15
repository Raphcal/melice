//
//  FileManager.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 15/12/2021.
//

import Foundation

public extension MELFileManager {
    static func initializeSharedInstance() {
        let fileManager: UnsafeMutablePointer<MELFileManager> = MELFileManagerGetSharedInstance()
        #if os(iOS)
        MELFileManagerInitWithAssetsRoot(fileManager, Bundle.main.bundlePath)
        #else
        MELFileManagerInitWithAssetsRoot(fileManager, Bundle.main.bundlePath.appending("/Contents/Resources"))
        #endif
    }
}
