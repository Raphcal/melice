//
//  spritedefinition-test.c
//  shmup
//
//  Created by Raphaël Calabro on 04/03/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <melice.h>
#include <assert.h>

int main(int argc, char **argv) {
    MELFileManager *fileManager = MELFileManagerGetSharedInstance();
    MELFileManagerInitWithArguments(fileManager, argv);
    
    MELInputStream inputStream = MELFileManagerOpenAsset(fileManager, "map1-sprites.def");

    MELList(MELSpriteDefinition) definitions;
    if (inputStream.file) {
        definitions = MELSpriteDefinitionListMakeWithInputStream(&inputStream);
    } else {
        printf("File 'map1-sprites.def' was not found.\n");
        return EXIT_FAILURE;
    }
    MELInputStreamClose(&inputStream);
    
    assert(definitions.count == 1);
    
    MELSpriteDefinition definition = definitions.memory[0];
    assert(strcmp("Sprites", definition.name) == 0);
    
    MELAnimationDefinition standAnimation = definition.animations.memory[0];
    assert(strcmp("stand", standAnimation.name) == 0);
    assert(standAnimation.frameCount == 5);
    assert(standAnimation.type == MELAnimationTypePlayOnce);
    
    return EXIT_SUCCESS;
}
