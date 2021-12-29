//
//  ImagePaletteImage.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 29/12/2021.
//

import Foundation

public extension MELImagePaletteImage {
    static let empty = MELImagePaletteImage()

    var tileCount: Int32 {
        return size.width * size.height
    }

    subscript(x: Int, y: Int) -> Int32 {
        get {
            assert(indexIsValid(x: x, y: y), "Index \(x)x\(y) is out of bounds")
            return self.tiles![x + y * Int(self.size.width)]
        }
        set(newValue) {
            assert(indexIsValid(x: x, y: y), "Index \(x)x\(y) is out of bounds")
            self.tiles![x + y * Int(self.size.width)] = newValue
        }
    }
    subscript(point: MELIntPoint) -> Int32 {
        get {
            assert(indexIsValid(x: Int(point.x), y: Int(point.y)), "Point \(point) is out of bounds")
            return self.tiles![Int(point.x + point.y * self.size.width)]
        }
        set(newValue) {
            assert(indexIsValid(x: Int(point.x), y: Int(point.y)), "Point \(point) is out of bounds")
            self.tiles![Int(point.x + point.y * self.size.width)] = newValue
        }
    }
    func indexIsValid(x: Int, y: Int) -> Bool {
        return x >= 0 && x < self.size.width && y >= 0 && y < self.size.height
    }
}
