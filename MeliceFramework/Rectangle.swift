//
//  Rectanle.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 09/04/2021.
//

import Foundation

public extension MELRectangle {
    init(x: GLfloat, y: GLfloat, width: GLfloat, height: GLfloat) {
        self.init(origin: MELPoint(x: x, y: y), size: MELSize(width: width, height: height))
    }
}

public extension MELIntRectangle {
    init(x: Int32, y: Int32, width: Int32, height: Int32) {
        self.init(origin: MELIntPoint(x: x, y: y), size: MELIntSize(width: width, height: height))
    }
}
