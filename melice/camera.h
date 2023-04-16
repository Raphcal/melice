//
//  camera.h
//  melice
//
//  Created by Raphaël Calabro on 16/04/2023.
//

#ifndef camera_h
#define camera_h

#include "melstd.h"

#include "point.h"
#include "size.h"
#include "sprite.h"

void MELCameraSetCurrent(MELPoint * _Nullable camera);
MELPoint * _Nullable MELCameraGetCurrent(void);

void MELCameraSetSize(MELSize size);

MELBoolean MELCameraSpriteIsInView(MELSprite * _Nonnull sprite);
void MELCameraRemoveSpriteIfOutOfView(MELSprite * _Nonnull sprite);

#endif /* camera_h */
