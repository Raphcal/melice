//
//  Palette.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 08/12/2021.
//

import Foundation

public extension MELPaletteRef {
    var name: String {
        if let cString = self.pointee.name {
            return String(cString: cString)
        } else {
            return "Palette"
        }
    }

    var tileSize: MELIntSize {
        return self.pointee.tileSize
    }

    var count: UInt32 {
        return self.pointee.count
    }

    var columns: Int {
        return Int(self.pointee.columns)
    }

    func paint(tile: UInt32) -> UnsafeMutablePointer<UInt8>? {
        return self.pointee.class.pointee.paintTile(self, tile)
    }

    func paint(map: MELMap, areaToRender: MELIntSize) -> UnsafeMutablePointer<UInt8>? {
        return self.pointee.class.pointee.paintMap(self, map, areaToRender)
    }

    func paint(map: MELMutableMap, areaToRender: MELIntSize) -> UnsafeMutablePointer<UInt8>? {
        return self.pointee.class.pointee.paintMap(self, map.super, areaToRender)
    }
}
