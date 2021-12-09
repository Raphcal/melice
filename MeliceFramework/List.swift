//
//  List.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 08/12/2021.
//

import Foundation

public protocol MELList: RandomAccessCollection where Index == Int {
    static var empty: Self { get }

    var memory: UnsafeMutablePointer<Element>? { get }
    var count: Int { get }
    var capacity: Int { get }

    init(memory: UnsafeMutablePointer<Element>?, count: Int, capacity: Int)
    init(capacity: Int)
}

public extension MELList {
    var startIndex: Int { return 0 }
    var endIndex: Int { return count }

    subscript(index: Int) -> Iterator.Element {
        get { return memory![index] }
        set(newValue) { memory![index] = newValue }
    }

    init(capacity: Int) {
        self.init(memory: UnsafeMutablePointer<Element>.allocate(capacity: capacity), count: 0, capacity: capacity)
    }

    func index(after i: Int) -> Int {
        return i + 1
    }
    func index(before i: Index) -> Index {
        return i - 1
    }
    func formIndex(after i: inout Int) {
        i += 1
    }
    func formIndex(before i: inout Int) {
        i -= 1
    }
}

extension MELLayerList: MELList {
    public typealias Element = MELLayer
    public static let empty = MELLayerListEmpty
}
extension MELMapGroupList: MELList {
    public typealias Element = MELMapGroup
    public static let empty = MELMapGroupListEmpty
}
extension MELMutableMapList: MELList {
    public typealias Element = MELMutableMap
    public static let empty = MELMutableMapListEmpty
}
extension MELPaletteRefList: MELList {
    public typealias Element = MELPaletteRef?
    public static let empty = MELPaletteRefListEmpty
}
extension MELSpriteDefinitionList: MELList {
    public typealias Element = MELSpriteDefinition
    public static let empty = MELSpriteDefinitionListEmpty
}
extension MELSpriteInstanceList: MELList {
    public typealias Element = MELSpriteInstance
    public static let empty = MELSpriteInstanceListEmpty
}
