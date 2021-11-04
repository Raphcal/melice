//
//  mutablemap.c
//  melice
//
//  Created by Raphaël Calabro on 02/11/2021.
//

#include "mutablemap.h"

MELListImplement(MELMutableMap);

MELMap MELMutableMapToMELMap(MELMutableMap self) {
    return (MELMap) {
        self.size,
        self.backgroundColor,
        self.layers.memory,
        self.layers.count
    };
}
