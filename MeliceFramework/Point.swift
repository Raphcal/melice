//
//  Point.swift
//  melice
//
//  Created by Raphaël Calabro on 09/04/2021.
//

import Foundation

extension MELPoint: Equatable {
    /// Compares the two given point.
    /// - returns: `true` if the points are the same, `false` otherwise.
    public static func ==(lhs: MELPoint, rhs: MELPoint) -> Bool {
        return MELPointEquals(lhs, rhs)
    }
}

public extension MELPoint {
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

    /// Adds the given points.
    /// - returns: A point whose coordinates are `lhs.x + rhs.x` and `lhs.y + rhs.y`.
    static func +(lhs: MELPoint, rhs: MELPoint) -> MELPoint {
        return MELPointAdd(lhs, rhs)
    }
    /// Substracts the given points.
    /// - returns: A point whose coordinates are `lhs.x - rhs.x` and `lhs.y - rhs.y`.
    static func -(lhs: MELPoint, rhs: MELPoint) -> MELPoint {
        return MELPointSubstract(lhs, rhs)
    }
    /// Multiplies the given points.
    /// - returns: A point whose coordinates are `lhs.x * rhs.x` and `lhs.y * rhs.y`.
    static func *(lhs: MELPoint, rhs: MELPoint) -> MELPoint {
        return MELPointMultiply(lhs, rhs)
    }
    /// Multiplies the coordinates of the given points by the given value.
    /// - returns: A point whose coordinates are `lhs.x * rhs` and `lhs.y * rhs`.
    static func *(lhs: MELPoint, rhs: GLfloat) -> MELPoint {
        return MELPointMultiplyByValue(lhs, rhs)
    }
    /// Divides the given points.
    /// - returns: A point whose coordinates are `lhs.x / rhs.x` and `lhs.y / rhs.y`.
    static func /(lhs: MELPoint, rhs: MELPoint) -> MELPoint {
        return MELPointDivide(lhs, rhs)
    }
}
