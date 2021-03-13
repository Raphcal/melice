//
//  fade.c
//  shmup
//
//  Created by Raphaël Calabro on 23/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "fade.h"

#include <assert.h>
#include "director.h"

void MELFadeReload(MELScene * _Nonnull self) {
    // Noop.
}

MELFade MELFadeMake(void (* _Nullable willAppear)(MELFade * _Nonnull self), void (* _Nullable update)(MELFade * _Nonnull self, MELTimeInterval timeSinceLastUpdate), void (* _Nullable draw)(MELFade * _Nonnull self)) {
    return (MELFade) {
        (MELScene) {
            MELSceneDefaults,
            .reload = &MELFadeReload,
            .willAppear = (void (*)(MELScene *)) willAppear,
            .update = (void (*)(MELScene *, MELTimeInterval)) update,
            .draw = (void (*)(MELScene *)) draw
        },
        NULL,
        NULL
    };
}

void MELFadeEmptyUpdateWith(MELFade * _Nonnull self, MELTimeInterval timeSinceLastUpdate) {
    MELScene *nextScene = self->nextScene;
    (*nextScene->willAppear)(nextScene);
    
    MELDirector * _Nonnull director = MELDirectorGetCurrent();
    director->nextScene = nextScene;
}

MELFade MELFadeMakeEmpty(void) {
    return MELFadeMake(NULL, &MELFadeEmptyUpdateWith, NULL);
}

MELBoolean MELSceneIsFade(MELScene self) {
    return self.reload == &MELFadeReload;
}
