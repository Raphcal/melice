//
//  Animation.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 19/04/2022.
//

import Foundation

public extension MELAnimationRef {
    var definition: MELAnimationDefinition {
        return self.pointee.definition!.pointee
    }
}
