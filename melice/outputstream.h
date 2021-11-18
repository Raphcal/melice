//
//  outputstream.h
//  melice
//
//  Created by Raphaël Calabro on 22/11/2021.
//

#ifndef outputstream_h
#define outputstream_h

#include "melstd.h"

#include "point.h"
#include "color.h"

typedef struct {
    FILE * _Nullable file;
    uint8_t * _Nonnull buffer;
    int32_t count;
} MELOutputStream;

MELOutputStream MELOutputStreamOpen(const char * _Nonnull path);
int MELOutputStreamClose(MELOutputStream * _Nonnull self);
void MELOutputStreamFlush(MELOutputStream * _Nonnull self);

/**
 * Writes and returns one byte to the given output stream.
 *
 * @param self Output stream to write to.
 * @param byte Unsigned 8 bits integer to write.
 */
void MELOutputStreamWriteByte(MELOutputStream * _Nonnull self, uint8_t byte);

/**
 * Writes size bytes from the given source into the output stream.
 *
 * @param self Output stream to write to.
 * @param source Pointer where to read the read bytes from.
 * @param size Number of bytes to write.
 */
void MELOutputStreamWrite(MELOutputStream * _Nonnull self, void * _Nonnull source, size_t size);

/**
 * Write the given number of zero.
 *
 * @param self Output stream to write to.
 * @param size Number of bytes to skip.
 */
void MELOutputStreamWriteZero(MELOutputStream * _Nonnull self, size_t size);

/**
 * Writes a boolean to the given ouput stream.
 *
 * @param self Output stream to write to.
 * @param value Boolean value to write.
 */
void MELOutputStreamWriteBoolean(MELOutputStream * _Nonnull self, MELBoolean value);

/**
 * Writes a 32 bits signed integer to the given ouput stream.
 *
 * @param self Output stream to write to.
 * @param value 32 bits signed integer to write.
 */
void MELOutputStreamWriteInt(MELOutputStream * _Nonnull self, int32_t value);

/**
 * Writes a 16 bits signed integer to the given ouput stream.
 *
 * @param self Output stream to write to.
 * @param value 16 bits signed integer.
 */
void MELOutputStreamWriteShort(MELOutputStream * _Nonnull self, int16_t value);

void MELOutputStreamWriteUInt16(MELOutputStream * _Nonnull self, uint16_t value);
void MELOutputStreamWriteUInt32(MELOutputStream * _Nonnull self, uint32_t value);

/**
 * Writes a float to the given ouput stream.
 *
 * @param self Output stream to write to.
 * @param value float.
 */
void MELOutputStreamWriteFloat(MELOutputStream * _Nonnull self, float value);

/**
 * Writes a double to the given ouput stream.
 *
 * @param self Output stream to write to.
 * @param value double.
 */
void MELOutputStreamWriteDouble(MELOutputStream * _Nonnull self, double value);

/**
 * Writes a byte array to the given ouput stream.
 *
 * @param self Output stream to write to.
 * @param value Byte array to write.
 * @param count Number of elements to write.
 */
void MELOutputStreamWriteByteArray(MELOutputStream * _Nonnull self, uint8_t * _Nonnull value, int32_t count);

/**
 * Writes a byte array to the given ouput stream.
 *
 * @param self Output stream to write to.
 * @param value Byte array or NULL.
 * @param count Number of elements to write. Will be set to 0 if the array is NULL.
 *
 */
void MELOutputStreamWriteNullableByteArray(MELOutputStream * _Nonnull self, uint8_t * _Nullable value, int32_t count);

/**
 * Writes a 32 bits signed integer array to the given ouput stream.
 *
 * @param self Output stream to write to.
 * @param value 32 bits signed integer array.
 * @param count Number of elements to write.
 */
void MELOutputStreamWriteIntArray(MELOutputStream * _Nonnull self, int32_t * _Nonnull value, int32_t count);

/**
 * Writes the given UTF-8 string as an UTF-16 string to the given ouput stream.
 *
 * @param self Output stream to write to.
 * @param value UTF-8 string.
 */
void MELOutputStreamWriteString(MELOutputStream * _Nonnull self, char * _Nonnull value);

/**
 * Writes the given UTF-8 string as an UTF-16 string to the given ouput stream.
 *
 * @param self Output stream to write to.
 * @param value UTF-8 string or NULL.
 */
void MELOutputStreamWriteNullableString(MELOutputStream * _Nonnull self, char * _Nullable value);

/**
 * Writes a color to the given ouput stream.
 *
 * @param self Output stream to write to.
 * @param value Color to write.
 */
void MELOutputStreamWriteColor(MELOutputStream * _Nonnull self, MELColor value);

/**
 * Writes a point to the given ouput stream.
 *
 * @param self Output stream to write to.
 * @param value Point to write.
 */
void MELOutputStreamWritePoint(MELOutputStream * _Nonnull self, MELPoint value);

#endif /* outputstream_h */
