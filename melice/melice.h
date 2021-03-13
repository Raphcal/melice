//
//  melice.h
//  melice
//
//  Created by Raphaël Calabro on 13/03/2021.
//

#ifndef melice_h
#define melice_h

#include "geometry/point.h"
#include "geometry/quadrilateral.h"
#include "geometry/rectangle.h"
#include "geometry/size.h"
#include "io/filemanager.h"
#include "io/inputstream.h"
// TODO: Add remaining headers from io directory
#include "map/layer.h"
#include "map/map.h"
#include "map/maprenderer.h"
#include "scene/director.h"
#include "scene/fade.h"
#include "scene/scene.h"
#include "sprite/axe.h"
#include "sprite/direction.h"
#include "sprite/sprite.h"
#include "sprite/spriteinstance.h"
#include "sprite/spritemanager.h"
#include "sprite/animation/animation.h"
#include "sprite/animation/animationframe.h"
#include "sprite/animation/animationtype.h"
#include "sprite/animation/noanimation.h"
#include "sprite/animation/singleframeanimation.h"
#include "sprite/definition/animationdefinition.h"
#include "sprite/definition/spritedefinition.h"
#include "sprite/hitbox/hitbox.h"
#include "sprite/hitbox/statichitbox.h"
#include "sprite/hitbox/simplespritehitbox.h"
#include "sprite/hitbox/spritehitbox.h"
#include "sprite/hitbox/rotatedhitbox.h"
#include "sprite/motion/motion.h"
#include "sprite/motion/nomotion.h"
#include "surface/color.h"
#include "surface/referencepool.h"
#include "surface/renderer.h"
#include "surface/surface.h"
#include "surface/surfacearray.h"
#include "surface/texture.h"
#include "surface/textureatlas.h"
#include "util/list.h"
#include "util/melmath.h"
#include "util/operation.h"
#include "util/random.h"
#include "util/time.h"

#endif /* melice_h */
