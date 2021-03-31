//
//  scene.h
//  shmup
//
//  Created by Raphaël Calabro on 23/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef scene_h
#define scene_h

#include "melstd.h"

#include "color.h"

/**
 * A scene.
 */
typedef struct melscene MELScene;

struct melscene {
    
    /**
     * Color used to clear the screen before drawing the scene.
     */
    MELColor backgroundColor;
    
    /**
     * Load the assets and set up the scene when it is in unloaded state.
     *
     * @param self Scene instance.
     */
    void (* _Nonnull load)(MELScene * _Nonnull self);
    
    /**
     * Reset the scene when it is in loaded state.
     *
     * @param self Scene instance.
     */
    void (* _Nonnull reload)(MELScene * _Nonnull self);
    
    /**
     * Free the assets of the scene after it has been hidden.
     *
     * @param self Scene instance.
     */
    void (* _Nonnull unload)(MELScene * _Nonnull self);
    
    /**
     * Prepare the scene right before being shown on screen.
     *
     * @param self Scene instance.
     */
    void (* _Nonnull willAppear)(MELScene * _Nonnull self);
    
    /**
     * Update the scene.
     *
     * @param self Scene instance.
     * @param timeSinceLastUpdate Time elapsed in seconds between now and the last call to update.
     */
    void (* _Nonnull update)(MELScene * _Nonnull self, MELTimeInterval timeSinceLastUpdate);
    
    /**
     * Draw the scene content.
     *
     * @param self Scene instance.
     */
    void (* _Nonnull draw)(MELScene * _Nonnull self);

};

#define MELSceneDefaults .backgroundColor = {0, 0, 0, 1}, .load = &MELSceneNoop, .reload = &MELSceneNoop, .unload = &MELSceneNoop, .willAppear = &MELSceneNoop, .update = &MELSceneNoopUpdate, .draw = &MELSceneNoop

/**
 * Default load function.
 */
void MELSceneNoop(MELScene * _Nonnull self);

/**
 * Default noop function.
 */
void MELSceneNoopUpdate(MELScene * _Nonnull self, MELTimeInterval timeSinceLastUpdate);

/**
 * Returns an empty scene doing nothing.
 *
 * @return An empty scene.
 */
MELScene MELSceneMakeEmptyScene(void);

#endif /* scene_h */
