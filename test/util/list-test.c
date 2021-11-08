//
//  list-test.c
//  shmup
//
//  Created by Raphaël Calabro on 27/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#include <stdlib.h>
#include <melice.h>
#include <assert.h>

MELListDefine(MELPoint);

void testPush(void) {
    MELList(MELPoint) points = MELListMakeWithInitialCapacity(MELPoint, 2);
    assert(points.count == 0);
    
    MELListPush(points, MELPointMake(32, 48));
    assert(points.count == 1);
    assert(points.capacity == 2);
    
    MELListPush(points, MELPointMake(24, 24));
    assert(points.count == 2);
    assert(points.capacity == 2);
    
    MELListPush(points, MELPointMake(128, 8));
    assert(points.count == 3);
    assert(points.capacity == 4);
    
    assert(MELPointEquals(MELPointMake(32, 48), points.memory[0]));
    assert(MELPointEquals(MELPointMake(24, 24), points.memory[1]));
    assert(MELPointEquals(MELPointMake(128, 8), points.memory[2]));
    
    MELListDeinit(points);
}

void testRemove(void) {
    MELList(MELPoint) points = MELListMakeWithInitialCapacity(MELPoint, 2);
    assert(points.count == 0);
    
    MELListPush(points, MELPointMake(32, 48));
    MELListPush(points, MELPointMake(24, 24));
    MELListPush(points, MELPointMake(128, 8));
    assert(points.count == 3);
    assert(points.capacity == 4);
    
    MELListRemove(points, 1);
    assert(points.count == 2);
    assert(points.capacity == 4);
    
    assert(MELPointEquals(MELPointMake(32, 48), points.memory[0]));
    assert(MELPointEquals(MELPointMake(128, 8), points.memory[1]));
    
    MELListDeinit(points);
}

void testPop(void) {
    MELList(MELPoint) points = MELListMakeWithInitialCapacity(MELPoint, 2);
    assert(points.count == 0);
    
    MELListPush(points, MELPointMake(32, 48));
    MELListPush(points, MELPointMake(24, 24));
    MELListPush(points, MELPointMake(128, 8));
    assert(points.count == 3);
    
    assert(MELPointEquals(MELPointMake(128, 8), MELListPop(points)));
    assert(points.count == 2);
    assert(MELPointEquals(MELPointMake(24, 24), MELListPop(points)));
    assert(points.count == 1);
    assert(MELPointEquals(MELPointMake(32, 48), MELListPop(points)));
    assert(points.count == 0);
    
    MELListDeinit(points);
}

MELBoolean releasedLayers[3];

void testDeinitFunc(MELLayer * _Nonnull self) {
    releasedLayers[self->size.width] = true;
}

void testDeinit(void) {
    MELList(MELLayer) layers = MELListMake(MELLayer);
    assert(layers.count == 0);
    
    MELLayer layer = { NULL, 0, 32, 0, NULL, MELPointMake(1, 1) };
    MELListPush(layers, layer);
    
    layer = (MELLayer) { NULL, 1, 22, 0, NULL, MELPointMake(1, 1)};
    MELListPush(layers, layer);
    
    layer = (MELLayer) { NULL, 2, 12, 0, NULL, MELPointMake(1, 1)};
    MELListPush(layers, layer);
    
    MELLayer *memory = (MELLayer *)layers.memory;
    assert(memory[0].size.width == 0);
    assert(memory[0].size.height == 32);
    assert(memory[1].size.width == 1);
    assert(memory[1].size.height == 22);
    assert(memory[2].size.width == 2);
    assert(memory[2].size.height == 12);
    
    releasedLayers[0] = false;
    releasedLayers[1] = false;
    releasedLayers[2] = false;
    
    MELListDeinitWithDeinitFunction(layers, testDeinitFunc);
    assert(layers.memory == NULL);
    assert(layers.count == 0);
    assert(layers.capacity == 0);
    
    assert(releasedLayers[0] = true);
    assert(releasedLayers[1] = true);
    assert(releasedLayers[2] = true);
}

MELList(MELPoint) getList(MELList(MELPoint) points) {
    return points;
}

void testAffectation(void) {
    MELList(MELPoint) points = getList((MELList(MELPoint)) MELListMake(MELPoint));
    MELList(MELPoint) list2 = points;
    MELListDeinit(list2);
}

void editList(MELList(MELPoint) *points) {
    MELListRefPush(points, MELPointMake(32, 48));
    MELListRefPush(points, MELPointMake(24, 24));
    MELListRefPush(points, MELPointMake(128, 8));
}

void testRemoteEdits(void) {
    MELList(MELPoint) points = MELListMake(MELPoint);
    editList(&points);
    
    assert(points.count == 3);
    assert(MELPointEquals(MELPointMake(32, 48), points.memory[0]));
    assert(MELPointEquals(MELPointMake(24, 24), points.memory[1]));
    assert(MELPointEquals(MELPointMake(128, 8), points.memory[2]));
    
    MELList(MELPoint) *ref = &points;
    MELListRefDeinit(ref);
    assert(points.memory == NULL);
    assert(points.count == 0);
    assert(points.capacity == 0);
}

int main(int argc, char **argv) {
    testPush();
    testRemove();
    testPop();
    testDeinit();
    testAffectation();
    testRemoteEdits();
    return EXIT_SUCCESS;
}
