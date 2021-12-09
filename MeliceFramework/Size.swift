//
//  Size.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 09/12/2021.
//

import Foundation

public extension MELSize {
    static func + (lhs: MELSize, rhs: MELSize) -> MELSize {
        return MELSizeAdd(lhs, rhs)
    }
}
