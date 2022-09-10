//
//  degreeanimationdefinitionframestable.c
//  melice
//
//  Created by Raphaël Calabro on 22/07/2022.
//

#include "degreeanimationdefinitionframestable.h"

MELKeyValueTableImplement(MELDegrees, MELAnimationDefinitionFrames);

MELDegreesMELAnimationDefinitionFramesTable MELDegreesMELAnimationDefinitionFramesTableMakeWithTable(MELDegreesMELAnimationDefinitionFramesTable other) {
    MELDegreesMELAnimationDefinitionFramesTable self = {};
    MELDegreesMELAnimationDefinitionFramesTableEntryList entries = MELDegreesMELAnimationDefinitionFramesTableEntries(&other);
    for (size_t index = 0; index < entries.count; index++) {
        MELDegreesMELAnimationDefinitionFramesTableEntry entry = entries.memory[index];
        MELDegreesMELAnimationDefinitionFramesTablePut(&self, entry.key, MELAnimationDefinitionFramesMakeWithAnimationDefinitionFrames(entry.value));
    }
    MELDegreesMELAnimationDefinitionFramesTableEntryListDeinit(&entries);
    return self;
}
