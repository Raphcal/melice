//
//  ai.h
//  shmup
//
//  Created by Raphaël Calabro on 27/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef motion_h
#define motion_h

#include "../../melstd.h"

#include "../sprite.h"

typedef struct melmotion MELMotion;

/**
 * Class of a motion.
 */
typedef struct {
    
    /**
     * Initialize the motion for the given sprite.
     *
     * @param self Motion instance.
     * @param sprite Sprite controlled by this motion.
     */
    void (* _Nonnull load)(MELMotion * _Nonnull self, MELSprite * _Nonnull sprite);
    
    /**
     * Deinitialize the motion.
     *
     * @param self Motion instance.
     * @param sprite Sprite controlled by this motion.
     */
    void (* _Nonnull unload)(MELMotion * _Nonnull self, MELSprite * _Nonnull sprite);
    
    /**
     * Updates and moves the sprite.
     *
     * @param self Motion instance.
     * @param sprite Sprite controlled by this motion.
     * @param timeSinceLastUpdate Time elapsed in seconds between now and the last call to update.
     */
    void (* _Nonnull update)(MELMotion * _Nonnull self, MELSprite * _Nonnull sprite, MELTimeInterval timeSinceLastUpdate);

} MELMotionClass;

struct melmotion {
    
    /**
     * Class of this AI. Defines the virtual functions.
     */
    const MELMotionClass * _Nonnull class;

};

#define MELMotionClassDefaults .load = &MELMotionLoadUnload, .unload = &MELMotionLoadUnload, .update = &MELMotionUpdate

/**
 * Default load/unload function.
 */
void MELMotionLoadUnload(MELMotion * _Nonnull self, MELSprite * _Nonnull sprite);

/**
 * Default update function.
 */
void MELMotionUpdate(MELMotion * _Nonnull self, MELSprite * _Nonnull sprite, MELTimeInterval timeSinceLastUpdate);

#endif /* motion_h */
