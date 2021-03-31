//
//  Named.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 08/12/2021.
//

import Foundation

public protocol Named {
    static var defaultName: String { get }
    var name: UnsafeMutablePointer<CChar>? { get }
    var nameAsString: String { get }
}

public extension Named {
    var nameAsString: String {
        if let name = name {
            return String(cString: name)
        } else {
            return Self.defaultName
        }
    }
}
