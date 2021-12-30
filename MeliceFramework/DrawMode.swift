//
//  DrawMode.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 29/12/2021.
//

import Foundation

extension MELDrawMode: OptionSet {
    public static let unset = MELDrawModeUnset
    public static let texture = MELDrawModeTexture
    public static let color = MELDrawModeColor
    public static let index = MELDrawModeIndex
}
