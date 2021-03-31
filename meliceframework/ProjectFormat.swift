//
//  Project.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 08/12/2021.
//

import Foundation

enum ProjectFormatError: Error {
    case unableToOpenProject
}

extension MELProjectFormat {
    public mutating func openProject(data: Data) -> MELProject? {
        var project = MELProject()
        do {
            try openProject(data: data, project: &project)
            return project
        } catch {
            return nil
        }
    }

    public mutating func openProject(data: Data, project: inout MELProject) throws {
        let mutableData = NSMutableData(data: data)
        var inputStream = MELInputStreamInitWithBytes(mutableData.mutableBytes, mutableData.length)

        let result = self.class.pointee.openProject!(&self, &inputStream, &project)
        MELInputStreamDeinit(&inputStream)

        if !result {
            throw ProjectFormatError.unableToOpenProject
        }
    }
}
