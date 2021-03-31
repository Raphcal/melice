//
//  surfacememory.c
//  shmup
//
//  Created by Raphaël Calabro on 28/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "surfacememory.h"

void MELSurfaceMemoryAppendRectangle(GLfloat * _Nonnull memory, MELRectangle rectangle) {
    const GLfloat left = rectangle.origin.x;
    const GLfloat top = rectangle.origin.y;
    const GLfloat width = rectangle.size.width;
    const GLfloat height = rectangle.size.height;
    
    // Bas gauche
    memory[0] = left;
    memory[1] = top + height;
    
    // (idem)
    memory[2] = left;
    memory[3] = top + height;
    
    // Bas droite
    memory[4] = left + width;
    memory[5] = top + height;
    
    // Haut gauche
    memory[6] = left;
    memory[7] = top;
    
    // Haut droite
    memory[8] = left + width;
    memory[9] = top;
    
    // (idem)
    memory[10] = left + width;
    memory[11] = top;
}

void MELSurfaceMemoryAppendQuadrilateral(GLfloat * _Nonnull memory, MELQuadrilateral quadrilateral) {
    memory[0] = quadrilateral.bottomLeft.x;
    memory[1] = -quadrilateral.bottomLeft.y;
    
    // (idem)
    memory[2] = quadrilateral.bottomLeft.x;
    memory[3] = -quadrilateral.bottomLeft.y;
    
    // bas droite
    memory[4] = quadrilateral.bottomRight.x;
    memory[5] = -quadrilateral.bottomRight.y;
    
    // haut gauche
    memory[6] = quadrilateral.topLeft.x;
    memory[7] = -quadrilateral.topLeft.y;
    
    // haut droite
    memory[8] = quadrilateral.topRight.x;
    memory[9] = -quadrilateral.topRight.y;
    
    // (idem)
    memory[10] = quadrilateral.topRight.x;
    memory[11] = -quadrilateral.topRight.y;
}

void MELSurfaceMemoryAppendColor(GLubyte * _Nonnull memory, MELUInt8Color color) {
    memory[0] = color.red;
    memory[1] = color.green;
    memory[2] = color.blue;
    memory[3] = color.alpha;
}
