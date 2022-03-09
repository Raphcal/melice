//
//  MutableMap.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 08/12/2021.
//

import Foundation

extension MELMutableMap: Named {
    public static var defaultName = "Map"
}

extension MELMutableMap: Identifiable {
    public var id: UUID {
        UUID(uuid: self.uuid)
    }
}

extension MELMutableMap: Hashable {
    public static func == (lhs: MELMutableMap, rhs: MELMutableMap) -> Bool {
        return lhs.hashValue == rhs.hashValue
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(self.nameAsString)
        hasher.combine(MELColorToRGBAUInt32Color(self.backgroundColor))
        hasher.combine(self.size.width)
        hasher.combine(self.size.height)
        hasher.combine(self.layers.count)
    }
}

public extension MELMutableMap {
    var layers: MELLayerList {
        get { self.super.layers }
        set { self.super.layers = newValue }
    }

    var size: MELIntSize {
        get { self.super.size }
        set { self.super.size = newValue }
    }

    var backgroundColor: MELColor {
        get { self.super.backgroundColor }
        set { self.super.backgroundColor = newValue }
    }

    func locationInLayer(_ location: MELIntPoint, layerIndex: Int, camera: MELPoint) -> MELIntPoint {
        return MELIntPoint((location + camera * layers[layerIndex].scrollRate) / palette.tileSize)
    }
}
