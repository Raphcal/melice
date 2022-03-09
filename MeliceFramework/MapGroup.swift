//
//  MapGroup.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 11/01/2022.
//

import Foundation

extension MELMapGroup: Identifiable {
    public var id: UUID {
        UUID(uuid: self.uuid)
    }
}

public extension MELMapGroup {
    subscript(mapId: UUID) -> MELMutableMap {
        get {
            var queue = [self]
            while !queue.isEmpty {
                var nextQueue = [MELMapGroup]()
                let group = queue.removeFirst()
                if let map = group.maps.first(where: { $0.id == mapId }) {
                    return map
                }
                nextQueue.append(contentsOf: group.children)
                queue = nextQueue
            }
            preconditionFailure("No map with UUID \(mapId) found")
        }
        set(newValue) {
            var queue = [self]
            while !queue.isEmpty {
                var nextQueue = [MELMapGroup]()
                var group = queue.removeFirst()
                if let index = group.maps.firstIndex(where: { $0.id == mapId }) {
                    group.maps[index] = newValue
                    return
                }
                nextQueue.append(contentsOf: group.children)
                queue = nextQueue
            }
        }
    }
}
