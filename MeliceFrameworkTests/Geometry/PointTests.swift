//
//  PointTests.swift
//  MeliceFrameworkTests
//
//  Created by Raphaël Calabro on 09/04/2021.
//

import XCTest
import Melice
import MeliceFramework

class PointTests: XCTestCase {

    func testAdd() throws {
        XCTAssertEqual(MELPoint(x: 12, y: 34) + MELPoint(x: 56, y: 78), MELPoint(x: 68, y: 112))
    }

    func testSubstract() throws {
        XCTAssertEqual(MELPoint(x: 56, y: 78) - MELPoint(x: 12, y: 34), MELPoint(x: 44, y: 44))
    }

    func testMultiply() throws {
        XCTAssertEqual(MELPoint(x: 2, y: 3) * MELPoint(x: 4, y: 5), MELPoint(x: 8, y: 15))
    }

    func testMultiplyByValue() throws {
        XCTAssertEqual(MELPoint(x: 2, y: 3) * 4, MELPoint(x: 8, y: 12))
    }

    func testDivide() throws {
        XCTAssertEqual(MELPoint(x: 4, y: 27) / MELPoint(x: 2, y: 3), MELPoint(x: 2, y: 9))
    }

}
