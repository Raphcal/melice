//
//  fade.h
//  shmup
//
//  Created by Raphaël Calabro on 23/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef fade_h
#define fade_h

#include "melstd.h"

#include "scene.h"

/**
 * A fade. Describes a transition between two scenes.
 */
typedef struct {
    
    /**
     * Fade scene.
     */
    MELScene super;

    /**
     * Scene to transition from.
     */
    MELScene * _Nullable oldScene;

    /**
     * Scene to transition to.
     */
    MELScene * _Nullable nextScene;

} MELFade;

/**
 * Returns a fade scene with the given functions.
 *
 * @param willAppear Function called before a fade between two scenes.
 * @param update Function called to update the fade. Director.nextScene should be set at the end of the fade.
 * @param draw Function called to draw the fade.
 * @return A fade.
 */
MELFade MELFadeMake(void (* _Nullable willAppear)(MELFade * _Nonnull self), void (* _Nullable update)(MELFade * _Nonnull self, MELTimeInterval timeSinceLastUpdate), void (* _Nullable draw)(MELFade * _Nonnull self));

/**
 * Returns an empty fade scene.
 * 
 * @return An empty fade.
 */
MELFade MELFadeMakeEmpty(void);

/**
 * Returns true if the given scene is a fade scene.
 *
 * @param self Scene instance to check.
 * @return true if the given scene is a fade, false otherwise.
 */
MELBoolean MELSceneIsFade(MELScene self);

#endif /* fade_h */
