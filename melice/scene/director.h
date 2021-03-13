//
//  director.h
//  shmup
//
//  Created by Raphaël Calabro on 23/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef director_h
#define director_h

#include "../melstd.h"

#include "scene.h"
#include "fade.h"

/**
 * A director.
 * Handle fades and changes between scenes.
 */
typedef struct {
    
    /**
     * Scene currently displayed on screen.
     * Will be freed when replaced by an other scene.
     */
    MELScene * _Nullable scene;
    
    /**
     * If not null, scene to fade to.
     */
    MELScene * _Nullable nextScene;
    
    /**
     * Fade scene to use during scene changes.
     */
    MELFade fade;

} MELDirector;

/**
 * Returns a director with the given fade scene.
 *
 * @param fadeScene Fade scene to use during scene changes.
 * @return A director.
 */
#define MELDirectorMake(fadeScene) ((MELDirector) {NULL, NULL, fadeScene})

/**
 * Free the properties of the given director.
 *
 * @param self Director instance.
 */
void MELDirectorDeinit(MELDirector * _Nonnull self);

/**
 * Sets the given director as the current shared instance.
 *
 * @param director Director to set as the current shared instance or NULL.
 */
void MELDirectorSetCurrent(MELDirector * _Nullable director);

/**
 * Returns the current shared director instance.
 *
 * @return The current shared director instance.
 */
MELDirector * _Nullable MELDirectorGetCurrent(void);

/**
 * Initialize the given director and sets the initial scene.
 *
 * @param self Director instance.
 * @param firstScene Initial scene of the director. Will be set as the current scene and its load and willAppear methods will be called.
 */
void MELDirectorStartWithScene(MELDirector * _Nonnull self, MELScene * _Nonnull firstScene);

/**
 * Restore the current scene of the given director.
 * The reload method of the current scene will be called.
 *
 * @param self Director instance.
 */
void MELDirectorRestartCurrentScene(MELDirector *_Nonnull self);

/**
 * Updates the current scene of the given director.
 *
 * @param self Director instance.
 * @param timeSinceLastUpdate Time in seconds since the last call to this method.
 */
void MELDirectorUpdateWithTimeSinceLastUpdate(MELDirector *_Nonnull self, MELTimeInterval timeSinceLastUpdate);

/**
 * Draw the current scene of the given director.
 *
 * @param self Director instance.
 */
void MELDirectorDraw(MELDirector self);

#endif /* director_h */
