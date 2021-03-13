//
//  color.c
//  shmup
//
//  Created by Raphaël Calabro on 31/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "color.h"

MELUInt32Color MELColorToUInt32Color(MELColor self) {
    return ((MELUInt32Color)(self.blue * 255)) + ((MELUInt32Color)(self.green * 255) << 8) + ((MELUInt32Color)(self.red * 255) << 16) + ((MELUInt32Color)(self.alpha * 255) << 24);
}
