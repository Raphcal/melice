//
//  degreeanimationdefinitionframestable.h
//  melice
//
//  Created by Raphaël Calabro on 22/07/2022.
//

#ifndef degreeanimationdefinitionframestable_h
#define degreeanimationdefinitionframestable_h

#include "melstd.h"

#include "animationdefinitionframes.h"
#include "primitives.h"
#include "keyvaluetable.h"

MELKeyValueTableDefine(MELDegrees, MELAnimationDefinitionFrames);

MELDegreesMELAnimationDefinitionFramesTable MELDegreesMELAnimationDefinitionFramesTableMakeWithTable(MELDegreesMELAnimationDefinitionFramesTable other);

#endif /* degreeanimationdefinitionframestable_h */
