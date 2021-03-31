//
//  List.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 08/12/2021.
//

import Foundation

public protocol MELList: RandomAccessCollection where Index == Int {
    var memory: UnsafeMutablePointer<Element>? { get }
    var count: Int { get }
    var capacity: Int { get }

    init(memory: UnsafeMutablePointer<Element>?, count: Int, capacity: Int)
}

public extension MELList {
    var startIndex: Int { return 0 }
    var endIndex: Int { return count }

    subscript(index: Int) -> Iterator.Element {
        get { return memory![index] }
        set(newValue) { memory![index] = newValue }
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
}
extension MELMapGroupList: MELList {
    public typealias Element = MELMapGroup
}
extension MELMutableMapList: MELList {
    public typealias Element = MELMutableMap
}
