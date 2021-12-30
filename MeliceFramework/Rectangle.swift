//
//  Rectanle.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 09/04/2021.
//

import Foundation

public extension MELRectangle {
    static let zero = MELRectangleZero

    init(x: GLfloat, y: GLfloat, width: GLfloat, height: GLfloat) {
        self.init(origin: MELPoint(x: x, y: y), size: MELSize(width: width, height: height))
    }

    init(_ a: MELPoint, _ b: MELPoint) {
        self = MELRectangleMakeWithPoints(a, b)
    }

    func contains(_ point: MELPoint) -> Bool {
        return MELRectangleContainsPoint(self, point)
    }

    func intersects(with rectangle: MELRectangle) -> Bool {
        return MELRectangleIntersectsWithRectangle(self, rectangle)
    }

    static func + (lhs: MELRectangle, rhs: MELPoint) -> MELRectangle {
        return MELRectangle(origin: lhs.origin + rhs, size: lhs.size)
    }
    static func + (lhs: MELRectangle, rhs: MELSize) -> MELRectangle {
        return MELRectangle(origin: lhs.origin, size: lhs.size + rhs)
    }
    static func - (lhs: MELRectangle, rhs: MELPoint) -> MELRectangle {
        return MELRectangle(origin: lhs.origin - rhs, size: lhs.size)
    }
    static func - (lhs: MELRectangle, rhs: MELSize) -> MELRectangle {
        return MELRectangle(origin: lhs.origin, size: lhs.size - rhs)
    }
}

extension MELRectangle: Equatable {
    public static func == (lhs: MELRectangle, rhs: MELRectangle) -> Bool {
        return MELRectangleEquals(lhs, rhs)
    }
}

public extension MELIntRectangle {
    static let zero = MELIntRectangleZero

    init(x: Int32, y: Int32, width: Int32, height: Int32) {
        self.init(origin: MELIntPoint(x: x, y: y), size: MELIntSize(width: width, height: height))
    }

    init(_ a: MELIntPoint, _ b: MELIntPoint) {
        self = MELIntRectangleMakeWithPoints(a, b)
    }

    func contains(_ point: MELIntPoint) -> Bool {
        return MELIntRectangleContainsPoint(self, point)
    }

    func intersects(with rectangle: MELIntRectangle) -> Bool {
        return MELIntRectangleIntersectsWithRectangle(self, rectangle)
    }

    static func + (lhs: MELIntRectangle, rhs: MELIntPoint) -> MELIntRectangle {
        return MELIntRectangle(origin: lhs.origin + rhs, size: lhs.size)
    }
    static func + (lhs: MELIntRectangle, rhs: MELIntSize) -> MELIntRectangle {
        return MELIntRectangle(origin: lhs.origin, size: lhs.size + rhs)
    }
    static func - (lhs: MELIntRectangle, rhs: MELIntPoint) -> MELIntRectangle {
        return MELIntRectangle(origin: lhs.origin - rhs, size: lhs.size)
    }
    static func - (lhs: MELIntRectangle, rhs: MELIntSize) -> MELIntRectangle {
        return MELIntRectangle(origin: lhs.origin, size: lhs.size - rhs)
    }
}

extension MELIntRectangle: Equatable {
    public static func == (lhs: MELIntRectangle, rhs: MELIntRectangle) -> Bool {
        return MELIntRectangleEquals(lhs, rhs)
    }
}

