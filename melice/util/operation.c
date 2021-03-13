//
//  operation.c
//  shmup
//
//  Created by Raphaël Calabro on 08/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "operation.h"

#include <string.h>
#include "melmath.h"
#include "../sprite/sprite.h"

MELOperation MELOperationMakeWithInputStream(MELInputStream * _Nonnull inputStream) {
    MELOperation self;
    self.code = MELInputStreamReadByteArray(inputStream, &self.count);
    return self;
}

void MELOperationDeinit(MELOperation * _Nonnull operation) {
    free(operation->code);
    operation->code = NULL;
    operation->count = 0;
}

typedef enum {
    MELByteCodeAdd = 0x2b,
    MELByteCodeSubstract = 0x2d,
    MELByteCodeMultiply = 0x2a,
    MELByteCodeDivide = 0x2f,
    MELByteCodePow = 0x5e,
    MELByteCodeNegative = 0x6e,
    MELByteCodeConstant = 0x43,
    MELByteCodeX = 0x78,
    MELByteCodePi = 0x70,
    MELByteCodeE = 0x65,
    MELByteCodeMinimum = 0x6d,
    MELByteCodeMaximum = 0x4d,
    MELByteCodeCosinus = 0x63,
    MELByteCodeSinus = 0x73,
    MELByteCodeSquareRoot = 0x53,
    MELByteCodeZoom = 0x7a,
    MELByteCodeSpriteVariable = 0x76,
    MELByteCodeSpriteDirection = 0x64,
    MELByteCodeSpriteHitboxTop = 0x68
} MELByteCode;

MELList(GLfloat) MELOperationExecute(MELOperation self, GLfloat x, MELSprite * _Nullable sprite) {
    int32_t index = 0;
    MELList(GLfloat) stack = (MELList(GLfloat)) MELListMakeWithInitialCapacity(GLfloat, self.count);
    
    GLfloat operand;
    
    while (index < self.count) {
        switch ((MELByteCode)self.code[index++]) {
            case MELByteCodeAdd:
                MELListPush(stack, MELListPop(stack) + MELListPop(stack));
                break;
                
            case MELByteCodeSubstract:
                operand = MELListPop(stack);
                MELListPush(stack, MELListPop(stack) - operand);
                break;
                
            case MELByteCodeMultiply:
                MELListPush(stack, MELListPop(stack) * MELListPop(stack));
                break;
                
            case MELByteCodeDivide:
                operand = MELListPop(stack);
                MELListPush(stack, MELListPop(stack) / operand);
                break;
                
            case MELByteCodePow:
                operand = MELListPop(stack);
                MELListPush(stack, powf(MELListPop(stack), operand));
                break;
                
            case MELByteCodeNegative:
                MELListPush(stack, -MELListPop(stack));
                break;
                
            case MELByteCodeConstant:
                memcpy(&operand, self.code + index, sizeof(GLfloat));
                index += sizeof(GLfloat);
                MELListPush(stack, operand);
                break;
                
            case MELByteCodeX:
                MELListPush(stack, x);
                break;
                
            case MELByteCodePi:
                MELListPush(stack, M_PI);
                break;
                
            case MELByteCodeE:
                MELListPush(stack, M_E);
                break;
                
            case MELByteCodeMinimum:
                MELListPush(stack, MELFloatMin(MELListPop(stack), MELListPop(stack)));
                break;
                
            case MELByteCodeMaximum:
                MELListPush(stack, MELFloatMax(MELListPop(stack), MELListPop(stack)));
                break;
                
            case MELByteCodeCosinus:
                MELListPush(stack, cosf(MELListPop(stack)));
                break;
                
            case MELByteCodeSinus:
                MELListPush(stack, sinf(MELListPop(stack)));
                break;
                
            case MELByteCodeSquareRoot:
                operand = MELListPop(stack);
                if (operand >= 0) {
                    MELListPush(stack, sqrtf(operand));
                } else {
                    MELListPush(stack, -1);
                }
                break;
                
            case MELByteCodeZoom:
                break;
                
            case MELByteCodeSpriteVariable:
                printf("SpriteVariable is not supported yet.\n");
                break;
                
            case MELByteCodeSpriteDirection:
                sprite->direction = (MELDirection)MELListPop(stack);
                break;
                
            case MELByteCodeSpriteHitboxTop:
                printf("SpriteHitboxTop is not supported yet.\n");
                break;
                
            default:
                break;
        }
    }
    
    return stack;
}
