//
//  Direction.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 14/04/2022.
//

import Foundation

enum MELDirectionError: Error {
    case badDirectionName(name: String)
}

public extension MELDirection {
    var value: GLfloat {
        return MELDirectionValueOf(self)
    }

    var reverse: MELDirection {
        return MELDirectionReverseOf(self)
    }

    var axe: MELAxe {
        return MELDirectionAxeOf(self)
    }

    static func named(_ name: String) throws -> MELDirection {
        switch name {
        case "up":
            return MELDirectionUp
        case "down":
            return MELDirectionDown
        case "left":
            return MELDirectionLeft
        case "right":
            return MELDirectionRight
        default:
            throw MELDirectionError.badDirectionName(name: name)
        }
    }
}
