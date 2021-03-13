//
//  list.h
//  shmup
//
//  Created by Raphaël Calabro on 26/02/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef list_h
#define list_h

#include "../melstd.h"

#include <string.h>

#define MELListDefine(type) /** List of type */ typedef struct mellist_##type { \
    /** Content of the list. */ \
    type * _Nullable memory; \
    /** Number of elements in the list. */ \
    size_t count; \
    /** Current capacity. */ \
    size_t capacity; \
} MELListOf##type

#define MELList(type) MELListOf##type

/**
 * Returns an empty list with the given initial capacity.
 *
 * @param type Element type.
 * @param initialCapacity Initial capacity of the list.
 * @return An empty list.
 */
#define MELListMakeWithInitialCapacity(type, initialCapacity) { malloc((initialCapacity) * sizeof(type)), 0, (initialCapacity) }

/**
 * Returns an empty list with an initial capacity of 10 elements.
 *
 * @param type Element type.
 * @return An empty list.
 */
#define MELListMake(type) MELListMakeWithInitialCapacity(type, 10)

/**
 * Returns an empty list with an initial capacity of 0.
 * Can be used to initialize a global list.
 *
 * @param type Element type.
 * @return An empty list.
 */
#define MELListMakeEmpty(type) { NULL, 0, 0 }

/**
 * Deinit and free the resources allocated for the given list.
 * The list itself is not freed.
 *
 * @param self List instance.
 */
#define MELListDeinit(self) { \
    free(self.memory); \
    self.memory = NULL; \
    self.count = 0; \
    self.capacity = 0; \
}

#define MELListRefDeinit(self) { \
    free(self->memory); \
    self->memory = NULL; \
    self->count = 0; \
    self->capacity = 0; \
}

/**
 * Deinit every element of the list and deinit the list.
 * The list itself is not freed.
 *
 * @param self List instance.
 * @param deinitFunc Deinit function. Should take a pointer to an element of the list type.
 */
#define MELListDeinitWithDeinitFunction(self, deinitFunction) { \
    for (size_t index = 0; index < self.count; index++) { \
        deinitFunction(self.memory + index); \
    } \
    MELListDeinit(self); \
}

#define MELListRefDeinitWithDeinitFunction(self, deinitFunction) { \
    for (size_t index = 0; index < self->count; index++) { \
        deinitFunction(self->memory + index); \
    } \
    MELListRefDeinit(self); \
}

/**
 * Make the given list memory grow to the given capacity.
 *
 * @param self List instance.
 * @param size Capacity to grow to.
 */
#define MELListGrow(self, size) { \
    const size_t newSize = size; \
    self.memory = realloc(self.memory, newSize * sizeof(*self.memory));\
    self.capacity = newSize;\
}

#define MELListRefGrow(self, size) { \
    const size_t newSize = size; \
    self->memory = realloc(self->memory, newSize * sizeof(*self->memory));\
    self->capacity = newSize;\
}

/**
 * Ensures that the given list can store the given number of elements.
 * If necessary, the list memory is grown.
 *
 * @param self List instance.
 * @param requiredCapacity Capacity required.
 */
#define MELListEnsureCapacity(self, required) { \
    const size_t requiredCapacity = required; \
    if (self.capacity < requiredCapacity) { \
        const size_t newCapacity = self.capacity + (self.capacity >> 1) + 1; \
        MELListGrow(self, newCapacity > requiredCapacity ? newCapacity : requiredCapacity); \
    } \
}

#define MELListRefEnsureCapacity(self, required) { \
    const size_t requiredCapacity = required; \
    if (self->capacity < requiredCapacity) { \
        const size_t newCapacity = self->capacity + (self->capacity >> 1) + 1; \
        MELListRefGrow(self, newCapacity > requiredCapacity ? newCapacity : requiredCapacity); \
    } \
}

/**
 * Adds the given value at the end of the given list.
 *
 * @param self List instance.
 * @param element Element to add.
 */

#define MELListPush(self, element) { \
    MELListEnsureCapacity(self, self.count + 1); \
    self.memory[self.count++] = element; \
}

#define MELListRefPush(self, element) { \
    MELListRefEnsureCapacity(self, self->count + 1); \
    self->memory[self->count++] = element; \
}

/**
 * Removes and returns the last element in the given list.
 *
 * @param self List instance.
 * @return The last element.
 */
#define MELListPop(self) self.memory[--self.count]

#define MELListRefPop(self) self->memory[--self->count]

#define MELListSet(self, index, element) { \
    const int propertyIndex = index; \
    if (propertyIndex >= self.count) { \
        MELListEnsureCapacity(self, propertyIndex + 1); \
        memset(self.memory + self.count, 0, sizeof(*self.memory) * (propertyIndex - self.count)); \
        self.count = propertyIndex + 1; \
    } \
    self.memory[propertyIndex] = element; \
}

/**
 * Removes the element located at the given index.
 *
 * @param self List instance.
 * @param index Index of the element to remove.
 */
#define MELListRemove(self, index) memmove(self.memory + index, self.memory + index + 1, ((self.count--) - index - 1) * sizeof(*self.memory))

#define MELListRefRemove(self, index) memmove(self->memory + index, self->memory + index + 1, ((self->count--) - index - 1) * sizeof(*self->memory))

/**
 * Removes the first element equals to the given element.
 *
 * @param self List instance.
 * @param element Element to remove.
 */
#define MELListRemoveElement(self, element) { \
    for (int index = 0; index < self.count; index++) { \
        if (self.memory[index] == element) { \
            MELListRemove(self, index); \
        } \
    } \
}

#endif /* list_h */
