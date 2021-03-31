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
