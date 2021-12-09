//
//  Project.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 08/12/2021.
//

import Foundation

enum ProjectFormatError: Error {
    case openOperationNotSupported
    case saveOperationNotSupported
    case unableToOpenProject
}

extension MELProjectFormat {
    public mutating func open(project: inout MELProject, from data: Data) throws {
        guard let openProject = self.class.pointee.openProject
        else {
            throw ProjectFormatError.openOperationNotSupported
        }
        let mutableData = NSMutableData(data: data)
        var inputStream = MELInputStreamInitWithBytes(mutableData.mutableBytes, mutableData.length)

        let result = openProject(&self, &inputStream, &project)
        MELInputStreamDeinit(&inputStream)

        if !result {
            throw ProjectFormatError.unableToOpenProject
        }
    }

    public mutating func save(project: MELProject) throws -> Data {
        guard let saveProject = self.class.pointee.saveProject
        else {
            throw ProjectFormatError.saveOperationNotSupported
        }
        var count = 0
        let bytes = saveProject(&self, project, &count)
        let data = Data(bytes: bytes, count: count)
        bytes.deallocate()
        return data
    }
}
