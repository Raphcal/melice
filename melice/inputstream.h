//
//  inputstream.h
//  shmup
//
//  Created by Raphaël Calabro on 25/01/2019.
//  Copyright © 2019 Raphaël Calabro. All rights reserved.
//

#ifndef inputstream_h
#define inputstream_h

#include "melstd.h"

#include "color.h"
#include "point.h"

typedef struct {
    FILE * _Nullable file;
    uint8_t * _Nonnull buffer;
    size_t size;
    size_t cursor;
} MELInputStream;

typedef enum {
    MELInputStreamSeekFromStart = SEEK_SET,
    MELInputStreamSeekFromCurrent = SEEK_CUR,
    MELInputStreamSeekFromEnd = SEEK_END
} MELInputStreamSeekFrom;

/**
 * Opens and returns an input stream on the given path with the given mode.
 *
 * @param path Path of the file to open. Relative to the current directory.
 * @return An inputstream.
 */
MELInputStream MELInputStreamOpen(const char * _Nonnull path);

/**
 * Closes the given input stream.
 * No read operation will be possible on the given input stream after calling this function.
 *
 * @param self Input stream instance.
 * @return 0 if close operation succeeded, an other value otherwise.
 */
int MELInputStreamClose(MELInputStream * _Nonnull self);

/**
 * Creates an inputstream containing a copy of the bytes from the given byte array.
 *
 * @param bytes Content of the stream.
 * @param count Number of bytes to copy.
 * @return An inputstream.
 */
MELInputStream MELInputStreamInitWithBytes(const void * _Nonnull bytes, size_t count);

/**
 * Free the byte buffer backing the inputstream.
 *
 * @param self Input stream instance to deinit.
 */
void MELInputStreamDeinit(MELInputStream * _Nonnull self);

/**
 * Seek the position of the current inputstream to the given offset, relative to the given position.
 *
 * @param self Input stream instance.
 * @param offset Offset to seek to.
 * @param from Start position to seek from.
 * @return 0 if the operation succeeded, an other value otherwise.
 */
int MELInputStreamSeek(MELInputStream * _Nonnull self, long offset, MELInputStreamSeekFrom from);

/**
 * Reads and returns one byte from the given input stream.
 *
 * @param self Input stream instance.
 * @return A byte or -1 if the input stream end has been reached or if the stream is closed.
 */
int MELInputStreamReadByte(MELInputStream * _Nonnull self);

/**
 * Reads size bytes from the given input stream into the given pointer.
 *
 * @param self Input stream to read from.
 * @param destination Pointer where to write the read bytes.
 * @param size Number of bytes to read.
 */
void MELInputStreamRead(MELInputStream * _Nonnull self, void * _Nonnull destination, size_t size);

/**
 * Skip the given number of bytes.
 *
 * @param self Input stream to read from.
 * @param size Number of bytes to skip.
 */
void MELInputStreamSkipBytes(MELInputStream * _Nonnull self, size_t size);

/**
 * Reads and returns a boolean from the given input stream.
 *
 * @param self Input stream instance.
 * @return A boolean.
 */
MELBoolean MELInputStreamReadBoolean(MELInputStream * _Nonnull self);

/**
 * Reads and returns a 32 bits signed integer from the given input stream.
 *
 * @param self Input stream instance.
 * @return A 32 bits signed integer.
 */
int32_t MELInputStreamReadInt(MELInputStream * _Nonnull self);

uint32_t MELInputStreamReadUInt32(MELInputStream * _Nonnull self);

/**
 * Reads and returns a 16 bits signed integer from the given input stream.
 *
 * @param self Input stream instance.
 * @return A 16 bits signed integer.
 */
int16_t MELInputStreamReadShort(MELInputStream * _Nonnull self);

/**
 * Reads and returns a float from the given input stream.
 *
 * @param self Input stream instance.
 * @return A float.
 */
float MELInputStreamReadFloat(MELInputStream * _Nonnull self);

/**
 * Reads and returns a double from the given input stream.
 *
 * @param self Input stream instance.
 * @return A double.
 */
double MELInputStreamReadDouble(MELInputStream * _Nonnull self);

/**
 * Reads and returns a byte array from the given input stream.
 *
 * @param self Input stream instance.
 * @param count Pointer to an integer where to write the size of the returned array.
 * @return A byte array. You are responsible for freeing the returned array.
 */
uint8_t * _Nonnull MELInputStreamReadByteArray(MELInputStream * _Nonnull self, int32_t * _Nonnull count);

/**
 * Reads and returns a byte array from the given input stream.
 *
 * @param self Input stream instance.
 * @param count Pointer to an integer where to write the size of the returned array. Will be set to 0 if the array is NULL.
 * @return A byte array or NULL. You are responsible for freeing the returned array.
 */
uint8_t * _Nullable MELInputStreamReadNullableByteArray(MELInputStream * _Nonnull self, int32_t * _Nonnull count);

/**
 * Reads and returns a 32 bits signed integer array from the given input stream.
 *
 * @param self Input stream instance.
 * @param count Pointer to an integer where to write the size of the returned array.
 * @return A 32 bits signed integer array. You are responsible for freeing the returned array.
 */
int32_t * _Nonnull MELInputStreamReadIntArray(MELInputStream * _Nonnull self, int32_t * _Nonnull count);

/**
 * Reads and returns an UTF-16 string from the given input stream.
 *
 * @param self Input stream instance.
 * @return An UTF-16 string. You are responsible for freeing the returned string.
 */
char * _Nonnull MELInputStreamReadString(MELInputStream * _Nonnull self);

/**
 * Reads and returns an UTF-16 string from the given input stream.
 *
 * @param self Input stream instance.
 * @return An UTF-16 string or NULL. You are responsible for freeing the returned string.
 */
char * _Nullable MELInputStreamReadNullableString(MELInputStream * _Nonnull self);

/**
 * Reads and returns a color from the given input stream.
 *
 * @param self Input stream instance.
 * @return A color. You are responsible for freeing the returned color.
 */
MELColor MELInputStreamReadColor(MELInputStream * _Nonnull self);

/**
 * Reads and returns a point from the given input stream.
 *
 * @param self Input stream instance.
 * @return A point. You are responsible for freeing the returned point.
 */
MELPoint MELInputStreamReadPoint(MELInputStream * _Nonnull self);

#endif /* inputstream_h */
