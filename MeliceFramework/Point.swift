//
//  Point.swift
//  melice
//
//  Created by Raphaël Calabro on 09/04/2021.
//

import Foundation

#if os(iOS)
import UIKit
#endif

extension MELPoint: Equatable {
    /// Compares the two given point.
    /// - returns: `true` if the points are the same, `false` otherwise.
    public static func ==(lhs: MELPoint, rhs: MELPoint) -> Bool {
        return MELPointEquals(lhs, rhs)
    }
}

public extension MELPoint {
    init(_ point: MELIntPoint) {
        self.init(x: GLfloat(point.x), y: GLfloat(point.y))
    }

    init(_ point: CGPoint) {
        self.init(x: GLfloat(point.x), y: GLfloat(point.y))
    }

    /// Returns the distance between the two points.
    /// - parameter other: An other point.
    /// - returns: The distance between the points.
    func distanceTo(_ other: MELPoint) -> GLfloat {
        return MELPointDistanceToPoint(self, other)
    }

    /// Returns the angle between the two points.
    /// - parameter other: An other point.
    /// - returns: The angle in radian between the two points.
    func angleTo(_ other: MELPoint) -> GLfloat {
        return MELPointDistanceToPoint(self, other)
    }
}

extension MELPoint: AdditiveArithmetic {
    public static let zero = MELPointZero

    /// Adds the given points.
    /// - returns: A point whose coordinates are `lhs.x + rhs.x` and `lhs.y + rhs.y`.
    public static func + (lhs: MELPoint, rhs: MELPoint) -> MELPoint {
        return MELPointAdd(lhs, rhs)
    }
    /// Adds the given points.
    /// - returns: A point whose coordinates are `lhs.x + rhs.x` and `lhs.y + rhs.y`.
    public static func + (lhs: MELIntPoint, rhs: MELPoint) -> MELPoint {
        return MELPoint(x: GLfloat(lhs.x) + rhs.x, y: GLfloat(lhs.y) + rhs.y)
    }
    /// Adds the given points.
    /// - returns: A point whose coordinates are `lhs.x + rhs.x` and `lhs.y + rhs.y`.
    public static func + (lhs: MELPoint, rhs: MELIntPoint) -> MELPoint {
        return MELPoint(x: lhs.x + GLfloat(rhs.x), y: lhs.y + GLfloat(rhs.y))
    }
    /// Substracts the given points.
    /// - returns: A point whose coordinates are `lhs.x - rhs.x` and `lhs.y - rhs.y`.
    public static func - (lhs: MELPoint, rhs: MELPoint) -> MELPoint {
        return MELPointSubstract(lhs, rhs)
    }
    /// Substracts the given points.
    /// - returns: A point whose coordinates are `lhs.x - rhs.x` and `lhs.y - rhs.y`.
    public static func - (lhs: MELIntPoint, rhs: MELPoint) -> MELPoint {
        return MELPoint(x: GLfloat(lhs.x) - rhs.x, y: GLfloat(lhs.y) - rhs.y)
    }
    /// Substracts the given points.
    /// - returns: A point whose coordinates are `lhs.x - rhs.x` and `lhs.y - rhs.y`.
    public static func - (lhs: MELPoint, rhs: MELIntPoint) -> MELPoint {
        return MELPoint(x: lhs.x - GLfloat(rhs.x), y: lhs.y - GLfloat(rhs.y))
    }
    /// Multiplies the given points.
    /// - returns: A point whose coordinates are `lhs.x * rhs.x` and `lhs.y * rhs.y`.
    public static func * (lhs: MELPoint, rhs: MELPoint) -> MELPoint {
        return MELPointMultiply(lhs, rhs)
    }
    /// Multiplies the given points.
    /// - returns: A point whose coordinates are `lhs.x * rhs.x` and `lhs.y * rhs.y`.
    public static func * (lhs: MELPoint, rhs: MELIntPoint) -> MELPoint {
        return MELPoint(x: lhs.x * GLfloat(rhs.x), y: lhs.y * GLfloat(rhs.y))
    }
    /// Multiplies the given points.
    /// - returns: A point whose coordinates are `lhs.x * rhs.x` and `lhs.y * rhs.y`.
    public static func * (lhs: MELIntPoint, rhs: MELPoint) -> MELPoint {
        return MELPoint(x: GLfloat(lhs.x) * rhs.x, y: GLfloat(lhs.y) * rhs.y)
    }
    /// Multiplies the coordinates of the given points by the given value.
    /// - returns: A point whose coordinates are `lhs.x * rhs` and `lhs.y * rhs`.
    public static func * (lhs: MELPoint, rhs: GLfloat) -> MELPoint {
        return MELPointMultiplyByValue(lhs, rhs)
    }
    public static func * (lhs: MELPoint, rhs: MELSize) -> MELPoint {
        return MELPoint(x: lhs.x * rhs.width, y: lhs.y * rhs.height)
    }
    public static func * (lhs: MELPoint, rhs: MELIntSize) -> MELPoint {
        return MELPoint(x: lhs.x * GLfloat(rhs.width), y: lhs.y * GLfloat(rhs.height))
    }
    /// Divides the given points.
    /// - returns: A point whose coordinates are `lhs.x / rhs.x` and `lhs.y / rhs.y`.
    public static func / (lhs: MELPoint, rhs: MELPoint) -> MELPoint {
        return MELPointDivide(lhs, rhs)
    }
    public static func / (lhs: MELPoint, rhs: MELIntPoint) -> MELPoint {
        return MELPoint(x: lhs.x / GLfloat(rhs.x), y: lhs.y / GLfloat(rhs.y))
    }
    public static func / (lhs: MELIntPoint, rhs: MELPoint) -> MELPoint {
        return MELPoint(x: GLfloat(lhs.x) / rhs.x, y: GLfloat(lhs.y) / rhs.y)
    }
    public static func / (lhs: MELPoint, rhs: MELIntSize) -> MELPoint {
        return MELPoint(x: lhs.x / GLfloat(rhs.width), y: lhs.y / GLfloat(rhs.height))
    }
    public static func / (lhs: MELPoint, rhs: MELSize) -> MELPoint {
        return MELPoint(x: lhs.x / rhs.width, y: lhs.y / rhs.height)
    }
}

extension MELIntPoint: Equatable {
    /// Compares the two given point.
    /// - returns: `true` if the points are the same, `false` otherwise.
    public static func ==(lhs: MELIntPoint, rhs: MELIntPoint) -> Bool {
        return MELIntPointEquals(lhs, rhs)
    }
}

extension MELIntPoint: AdditiveArithmetic {
    public static let zero = MELIntPointZero

    /// Adds the given points.
    /// - returns: A point whose coordinates are `lhs.x + rhs.x` and `lhs.y + rhs.y`.
    public static func + (lhs: MELIntPoint, rhs: MELIntPoint) -> MELIntPoint {
        return MELIntPointAdd(lhs, rhs)
    }

    /// Substracts the given points.
    /// - returns: A point whose coordinates are `lhs.x - rhs.x` and `lhs.y - rhs.y`.
    public static func - (lhs: MELIntPoint, rhs: MELIntPoint) -> MELIntPoint {
        return MELIntPointSubstract(lhs, rhs)
    }

    public static func * (lhs: MELIntPoint, rhs: MELIntPoint) -> MELIntPoint {
        return MELIntPoint(x: lhs.x * rhs.x, y: lhs.y * rhs.y)
    }
    public static func * (lhs: MELIntPoint, rhs: MELSize) -> MELIntPoint {
        return MELIntPoint(x: Int32(GLfloat(lhs.x) * rhs.width), y: Int32(GLfloat(lhs.y) * rhs.height))
    }
    public static func * (lhs: MELIntPoint, rhs: MELIntSize) -> MELIntPoint {
        return MELIntPoint(x: lhs.x * rhs.width, y: lhs.y * rhs.height)
    }

    public static func / (lhs: MELIntPoint, rhs: MELIntPoint) -> MELIntPoint {
        return MELIntPoint(x: lhs.x / rhs.x, y: lhs.y / rhs.y)
    }
    public static func / (lhs: MELIntPoint, rhs: MELSize) -> MELIntPoint {
        return MELIntPoint(x: Int32(GLfloat(lhs.x) / rhs.width), y: Int32(GLfloat(lhs.y) / rhs.height))
    }
    public static func / (lhs: MELIntPoint, rhs: MELIntSize) -> MELIntPoint {
        return MELIntPoint(x: lhs.x / rhs.width, y: lhs.y / rhs.height)
    }
}

public extension MELIntPoint {
    init(_ point: MELPoint) {
        self.init(x: Int32(point.x), y: Int32(point.y))
    }

    init(_ point: CGPoint) {
        self.init(x: Int32(point.x), y: Int32(point.y))
    }

    static func += (lhs: inout MELIntPoint, rhs: MELPoint) {
        lhs = MELIntPoint(x: lhs.x + Int32(rhs.x), y: lhs.y + Int32(rhs.y))
    }
}
