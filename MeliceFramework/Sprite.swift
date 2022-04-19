//
//  Sprite.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 19/04/2022.
//

import Foundation

public extension MELSpriteRef {
    var animation: MELAnimationRef {
        return self.pointee.animation
    }
    var definition: MELSpriteDefinition {
        return self.pointee.definition
    }
}
