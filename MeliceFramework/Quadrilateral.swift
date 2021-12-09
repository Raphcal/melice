//
//  Quadrilateral.swift
//  MeliceFramework
//
//  Created by Raphaël Calabro on 09/04/2021.
//

import Foundation

public extension MELQuadrilateral {
    var top: GLfloat {
        get { MELQuadrilateralGetTop(self) }
    }
    var right: GLfloat {
        get { MELQuadrilateralGetRight(self) }
    }
    var bottom: GLfloat {
        get { MELQuadrilateralGetBottom(self) }
    }
    var left: GLfloat {
        get { MELQuadrilateralGetLeft(self) }
    }
    var enclosingRectangle: MELRectangle {
        get { MELQuadrilateralGetEnclosingRectangle(self) }
    }
}
