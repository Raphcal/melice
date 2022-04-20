//
//  List.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 08/12/2021.
//

import Foundation

public protocol MELList: RandomAccessCollection, CustomDebugStringConvertible where Index == Int {
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

    var debugDescription: String {
        if count == 0 {
            return "[]"
        }
        var value = "[\(self[0])"
        for index in 1 ..< count {
            value += ", \(self[index])"
        }
        return value + "]"
    }

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

public protocol EquatableMELList: MELList, Equatable where Element: Equatable {
    // Empty.
}

extension EquatableMELList {
    public static func == (lhs: Self, rhs: Self) -> Bool {
        if lhs.count != rhs.count {
            return false
        }
        for index in 0 ..< Int(lhs.count) {
            if lhs[index] != rhs[index] {
                return false
            }
        }
        return true
    }
}

extension MELLayerList: EquatableMELList {
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
extension MELAnimationDefinitionList: MELList {
    public typealias Element = MELAnimationDefinition
    public static let empty = MELAnimationDefinitionListEmpty
}
extension MELDecoratorRefList: MELList {
    public typealias Element = MELDecoratorRef?
    public static let empty = MELDecoratorRefListEmpty
}
extension MELStringList: MELList {
    public typealias Element = MELString?
    public static let empty = MELStringListEmpty
}
extension MELConstStringList: MELList {
    public typealias Element = MELConstString
    public static let empty = MELConstStringListEmpty
}
extension MELBooleanList: EquatableMELList {
    public typealias Element = MELBoolean
    public static let empty = MELBooleanListEmpty

    public subscript(index: Int) -> Iterator.Element {
        get { return MELBooleanListGet(self, index) }
        set(newValue) { MELBooleanListSet(&self, index, newValue) }
    }

    public init(_ array: [MELBoolean]) {
        self.init(memory: nil, count: 0, capacity: 0)
        if !array.isEmpty {
            MELBooleanListEnsureCapacity(&self, array.count)
            self.count = array.count
            for index in 0 ..< array.count {
                self[index] = array[index]
            }
        }
    }

    public mutating func mutateAndSet(_ value: MELBoolean, at index: Int) {
        var newSelf = MELBooleanListMakeWithList(self)
        newSelf[index] = value
        var oldSelf = self
        self = newSelf
        DispatchQueue.main.async {
            MELBooleanListDeinit(&oldSelf)
        }
    }
}
extension MELByteList: EquatableMELList {
    public typealias Element = MELByte
    public static let empty = MELByteListEmpty
}
extension GLfloatList: EquatableMELList {
    public typealias Element = GLfloat
    public static let empty = GLfloatListEmpty
}
extension GLubyteList: EquatableMELList {
    public typealias Element = GLubyte
    public static let empty = GLubyteListEmpty
}
extension GLintList: EquatableMELList {
    public typealias Element = GLint
    public static let empty = GLintListEmpty
}
