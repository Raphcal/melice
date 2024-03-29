//
//  map-test.c
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
    
    MELInputStream inputStream = MELFileManagerOpenAsset(fileManager, "map0.grid");
    MELMap map;
    if (inputStream.file) {
        map = MELMapMakeWithInputStream(&inputStream);
    } else {
        printf("File 'map0.grid' was not found.\n");
        return EXIT_FAILURE;
    }
    MELInputStreamClose(&inputStream);
    
    assert(MELColorToUInt32Color(map.backgroundColor) == (uint32_t)0xff0099cc);
    assert(map.layers.count == 2);
    
    assert(strcmp("Background", map.layers.memory[0].name) == 0);
    assert(strcmp("HelloVita", map.layers.memory[1].name) == 0);
    
    MELMapDeinit(&map);
    
    return EXIT_SUCCESS;
}
