//
//  animation.h
//  shmup
//
//  Created by Raphaël Calabro on 28/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef animation_h
#define animation_h

#include "../../melstd.h"

#include "../definition/animationdefinition.h"
#include "animationframe.h"
#include "../sprite.h"

/**
 * Instance of an animation.
 */
typedef struct melanimation MELAnimation;

/**
 * Class of an animation.
 */
typedef struct {
    
    /**
     * Updates the animation.
     *
     * @param self Animation instance.
     * @param timeSinceLastUpdate Time elapsed in seconds between now and the last call to update.
     */
    void (* _Nonnull update)(MELAnimation * _Nonnull self, MELTimeInterval timeSinceLastUpdate);
    
    /**
     * Draws the current animation frame.
     *
     * @param self Animation instance.
     * @param sprite Sprite to animate.
     */
    void (* _Nonnull draw)(MELAnimation * _Nonnull self, MELSprite * _Nonnull sprite);
    
    /**
     * Starts the animation.
     *
     * @param self Animation instance.
     */
    void (* _Nonnull start)(MELAnimation * _Nonnull self);
    
    /**
     * Returns the next animation to use.
     *
     * @param self Animation instance.
     * @param nextAnimation Animation to transition to.
     * @return The next animation.
     */
    MELAnimation (* _Nonnull transitionToAnimation)(MELAnimation * _Nonnull self, MELAnimation nextAnimation);
    
} MELAnimationClass;

struct melanimation {

    /**
     * Class of this animation. Defines the virtual functions.
     */
    const MELAnimationClass * _Nonnull class;
    
    /**
     * Definition of this animation.
     */
    MELAnimationDefinition * _Nullable definition;
    
    /**
     * Index of the current frame.
     */
    int frameIndex;
    
    /**
     * Current frame.
     */
    MELAnimationFrame frame;
    
    /**
     * Speed of the animation.
     */
    MELTimeInterval speed;

};

#define MELAnimationClassDefaults .update = &MELAnimationUpdate, .draw = &MELAnimationDraw, .start = &MELAnimationStart, .transitionToAnimation = &MELAnimationTransitionToAnimation

/**
 * Returns an animation class with the given virtual functions.
 *
 * @param update Function called to update the animation.
 * @param draw Function called to draw the current animation frame.
 * @param start Function called to start the animation.
 * @param transitionToAnimation Function called to return the next animation to use.
 * @return An animation class.
 */
#define MELAnimationClassMake(update, draw, start, transitionToAnimation) ((MELAnimationClass) {update, draw, start, transitionToAnimation})

MELAnimation MELAnimationMake(const MELAnimationClass * _Nonnull class, MELAnimationDefinition * _Nullable definition);

MELAnimation * _Nonnull MELAnimationAlloc(MELAnimationDefinition * _Nullable definition);

/**
 * Default update function.
 */
void MELAnimationUpdate(MELAnimation * _Nonnull self, MELTimeInterval timeSinceLastUpdate);

/**
 * Default start function.
 */
void MELAnimationStart(MELAnimation * _Nonnull self);

/**
 * Default draw function.
 */
void MELAnimationDraw(MELAnimation * _Nonnull self, MELSprite * _Nonnull sprite);

/**
 * Default transitionToAnimation function.
 * Returns the given next animation as is.
 *
 * @param self Animation instance.
 * @param nextAnimation Animation to transition to.
 * @return The given animation.
 */
MELAnimation MELAnimationTransitionToAnimation(MELAnimation * _Nonnull self, MELAnimation nextAnimation);

MELTimeInterval MELAnimationFramesPerSecond(MELAnimation * _Nonnull self);

void MELAnimationSetFrameIndex(MELAnimation * _Nonnull self, int index);

#endif /* animation_h */
