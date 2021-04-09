//
//  Rectanle.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 09/04/2021.
//

import Melice

public extension MELRectangle {
    init(x: GLfloat, y: GLfloat, width: GLfloat, height: GLfloat) {
        self.init(origin: MELPoint(x: x, y: y), size: MELSize(width: width, height: height))
    }
}
