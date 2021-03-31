//
//  scene.c
//  shmup
//
//  Created by Raphaël Calabro on 23/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "scene.h"

MELScene MELSceneMakeEmptyScene(void) {
    return (MELScene){MELSceneDefaults};
}

void MELSceneNoop(MELScene * _Nonnull scene) {
    // Noop.
}

void MELSceneNoopUpdate(MELScene * _Nonnull scene, MELTimeInterval timeSinceLastUpdate) {
    // Noop.
}
