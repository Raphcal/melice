//
//  Size.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 09/12/2021.
//

import Foundation

public extension MELSize {
    init(_ size: MELIntSize) {
        self.init(width: GLfloat(size.width), height: GLfloat(size.height))
    }
    init(_ size: CGSize) {
        self.init(width: GLfloat(size.width), height: GLfloat(size.height))
    }
}

extension MELSize: AdditiveArithmetic {
    public static let zero = MELSizeZero
    
    public static func + (lhs: MELSize, rhs: MELSize) -> MELSize {
        return MELSizeAdd(lhs, rhs)
    }
    public static func - (lhs: MELSize, rhs: MELSize) -> MELSize {
        return MELSizeSubstract(lhs, rhs)
    }
    public static func * (lhs: MELSize, rhs: MELSize) -> MELSize {
        return MELSizeMultiply(lhs, rhs)
    }
    public static func / (lhs: MELSize, rhs: MELSize) -> MELSize {
        return MELSizeDivide(lhs, rhs)
    }
}

extension MELSize: Equatable {
    public static func == (lhs: MELSize, rhs: MELSize) -> Bool {
        return MELSizeEquals(lhs, rhs)
    }
}

public extension MELIntSize {
    init(_ size: MELSize) {
        self.init(width: Int32(size.width), height: Int32(size.height))
    }
}

extension MELIntSize: AdditiveArithmetic {
    public static let zero = MELIntSizeZero
    
    public static func + (lhs: MELIntSize, rhs: MELIntSize) -> MELIntSize {
        return MELIntSizeAdd(lhs, rhs)
    }
    public static func - (lhs: MELIntSize, rhs: MELIntSize) -> MELIntSize {
        return MELIntSizeSubstract(lhs, rhs)
    }
    public static func * (lhs: MELIntSize, rhs: MELIntSize) -> MELIntSize {
        return MELIntSizeMultiply(lhs, rhs)
    }
    public static func / (lhs: MELIntSize, rhs: MELIntSize) -> MELIntSize {
        return MELIntSizeDivide(lhs, rhs)
    }
}

extension MELIntSize: Equatable {
    public static func == (lhs: MELIntSize, rhs: MELIntSize) -> Bool {
        return MELIntSizeEquals(lhs, rhs)
    }
}
