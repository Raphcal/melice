//
//  shootingstyle.h
//  Kuroobi
//
//  Created by Raphaël Calabro on 27/01/2023.
//

#ifndef shootingstyle_h
#define shootingstyle_h

#include "melstd.h"

#include "shootingstyledefinition.h"
#include "sprite.h"
#include "point.h"
#include "spritemanager.h"

typedef enum {
    MELShootingStyleClassNameBurst,
    MELShootingStyleClassNameCircular,
    MELShootingStyleClassNameSimple,
    MELShootingStyleClassNameParticule,
    MELShootingStyleClassNameAimed,
    MELShootingStyleClassNameSinus,
} MELShootingStyleClassName;

typedef struct {
    MELShootingStyleClassName name;
    void (* _Nonnull createBullets)(MELShootingStyle * _Nonnull self, MELPoint origin, float angle, float initialDelta);
} MELShootingStyleClass;

const MELShootingStyleClass * _Nullable MELShootingStyleClassForName(MELShootingStyleClassName className);


typedef struct shootingstyle {
    const MELShootingStyleClass * _Nonnull class;
    const MELShootingStyleDefinition * _Nonnull definition;
    MELSpriteManager * _Nonnull spriteManager;
    void * _Nullable userdata;

    int layer;
    MELSpriteType type;

    MELTimeInterval shootInterval;
    unsigned int bulletAmount;
    unsigned int bulletAmountVariation;

    unsigned int inversionInterval;

    int shotsBeforePause;

    float baseAngle;

    MELBoolean canShootWhenHitPointsAreZero;
} MELShootingStyle;

extern const MELShootingStyle MELShootingStyleEmpty;

void MELShootingStyleInit(MELShootingStyle * _Nonnull self);
void MELShootingStyleShootFromSprite(MELShootingStyle * _Nonnull self, MELSprite * _Nonnull sprite, float angle, MELTimeInterval timeSinceLastUpdate);

MELSprite * _Nullable MELShootingStyleGetTarget(const MELShootingStyle * _Nonnull self);

float MELShootingStyleEaseNone(MELTimeInterval time);
float MELShootingStyleEaseInSine(MELTimeInterval time);
float MELShootingStyleEaseInQuad(MELTimeInterval time);
float MELShootingStyleEaseInCubic(MELTimeInterval time);
float MELShootingStyleEaseInQuart(MELTimeInterval time);
float MELShootingStyleEaseInQuint(MELTimeInterval time);
float MELShootingStyleEaseInExpo(MELTimeInterval time);
float MELShootingStyleEaseInBack(MELTimeInterval time);

#endif /* shootingstyle_h */
