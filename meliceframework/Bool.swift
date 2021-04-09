//
//  Bool.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 09/04/2021.
//

import Melice

public extension MELBoolean {
    /// Get a `MELBoolean` from a `Bool` value.
    static func from(_ bool: Bool) -> MELBoolean {
        return bool ? `true` : `false`
    }
    /// Get the corresponding `Bool` value from a `MELBoolean` value.
    var boolValue: Bool {
        get { self == `true` }
    }
}
