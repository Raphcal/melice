//
//  director.c
//  shmup
//
//  Created by Raphaël Calabro on 23/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include "director.h"

#include <assert.h>
#include "renderer.h"

MELDirector * _Nullable sharedDirector = NULL;

void MELDirectorDeinit(MELDirector * _Nonnull self) {
    MELScene *scene = self->scene;
    if (scene != NULL) {
        scene->unload(scene);
        free(scene);
        self->scene = NULL;
    }
    MELScene *nextScene = self->nextScene;
    if (nextScene != NULL) {
        nextScene->unload(nextScene);
        free(nextScene);
        self->nextScene = NULL;
    }
    self->fade.super.unload(&self->fade.super);
    self->fade = (MELFade) {
		MELSceneMakeEmptyScene(),
		NULL,
		NULL
	};
}

void MELDirectorSetCurrent(MELDirector * _Nullable director) {
    sharedDirector = director;
}

MELDirector * _Nullable MELDirectorGetCurrent(void) {
    return sharedDirector;
}

void MELDirectorStartWithScene(MELDirector * _Nonnull self, MELScene * _Nonnull firstScene) {
    assert(self != NULL);
    assert(firstScene != NULL);
    
    MELRendererInit();
    
    firstScene->load(firstScene);
    firstScene->willAppear(firstScene);
    self->scene = firstScene;
}

void MELDirectorRestartCurrentScene(MELDirector *self) {
    MELScene *scene = self->scene;
    scene->reload(scene);
}

void MELDirectorUpdateWithTimeSinceLastUpdate(MELDirector *self, MELTimeInterval timeSinceLastUpdate) {
    MELFade fade = self->fade;
    MELScene *scene = self->scene;
    MELScene *nextScene;

    if ((nextScene = self->nextScene) == NULL) {
        scene->update(scene, timeSinceLastUpdate);
    } else {
        if (!MELSceneIsFade(*scene)) {
            // Fading to the next scene.
            // TODO: Stop the music
            nextScene->load(nextScene);
            
            fade.oldScene = scene;
            fade.nextScene = nextScene;
            fade.super.load(&fade.super);
            
            nextScene = &fade.super;
        } else {
            fade.oldScene->unload(fade.oldScene);
            free(fade.oldScene);
            fade.oldScene = NULL;
            fade.nextScene = NULL;
        }
        self->scene = nextScene;
        self->nextScene = NULL;
    }
}

void MELDirectorDraw(MELDirector self) {
    MELScene *scene = self.scene;
    MELRendererClearWithColor(scene->backgroundColor);
    scene->draw(scene);
}
