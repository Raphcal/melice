//
//  SpriteInstance.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 04/01/2022.
//

import Foundation

public extension MELSpriteInstance {
    init(_ spriteInstance: MELSpriteInstance) {
        self = MELSpriteInstanceMakeWithSpriteInstance(spriteInstance)
    }
}
