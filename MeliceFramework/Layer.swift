//
//  MELLayer.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 08/12/2021.
//

import Foundation

extension MELLayer: Named {
    public static var defaultName = "Layer"
}

extension MELLayer: Equatable, Hashable {
    public static func == (lhs: MELLayer, rhs: MELLayer) -> Bool {
        return lhs.tileCount == rhs.tileCount &&
            memcmp(lhs.tiles, rhs.tiles, MemoryLayout<Int32>.stride * Int(lhs.tileCount)) == 0
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(self.nameAsString)
        hasher.combine(self.size.width)
        hasher.combine(self.size.height)
        hasher.combine(self.scrollRate.x)
        hasher.combine(self.scrollRate.y)
        hasher.combine(self.tileCount)
    }
}

extension MELLayer {
    public subscript(x: Int, y: Int) -> Int32 {
        get {
            assert(indexIsValid(x: x, y: y), "Index \(x)x\(y) is out of bounds")
            return self.tiles![x + y * Int(self.size.width)]
        }
        set(newValue) {
            self.tiles![x + y * Int(self.size.width)] = newValue
        }
    }
    public func indexIsValid(x: Int, y: Int) -> Bool {
        return x >= 0 && x < self.size.width && y >= 0 && y < self.size.height
    }
}
