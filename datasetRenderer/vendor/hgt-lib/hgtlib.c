#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hgtlib.h"

hgt_file_t *hgtCreateContext(const char *filename)
{
    // fixme: add error handling to all function calls to libc
    hgt_file_t *hgt = (hgt_file_t *)malloc(sizeof(hgt_file_t));

    if (!(hgt->fd = fopen(filename, "rb"))) // open file as binary
    {
        printf("Failed to load file %s\n", filename);
        return NULL;
    }

    hgt->filename = filename;

    fseek(hgt->fd, 0, SEEK_END);      // seek at the end
    hgt->bufferSize = ftell(hgt->fd); // read the file pointer
    fseek(hgt->fd, 0, SEEK_SET);      // seek at the start

    hgt->buffer = malloc(hgt->bufferSize);           // allocate a buffer to hold the buffer
    fread(hgt->buffer, hgt->bufferSize, 1, hgt->fd); // read the whole file

    hgt->sideLength = sqrt(hgt->bufferSize / sizeof(int16_t)); // the file is a square of 16-bit integers, the buffer size is its area

    return hgt;
}

void hgtDeleteContext(hgt_file_t *hgt)
{
    // close the file
    fclose(hgt->fd);

    // delete the used pointers
    free(hgt->buffer);
    free(hgt);
}

int16_t hgtReadElevationRaw(hgt_file_t *hgt, size_t offset)
{
    uint16_t be, le;

    be = hgt->buffer[offset];            // read the elevation value at the offset
    le = ((be & 0xFF) << 8) | (be >> 8); // swap endianness (x86_64 is little-endian)
                                         // fixme: don't assume this is x86_64!

    return (int16_t)le;
}

int16_t hgtReadElevation(hgt_file_t *hgt, double longitude, double latitude)
{
    // note to self: longitude is Y, latitude is X
    if (longitude <= 1.0 && latitude <= 1.0) // offset in file
    {
        uint16_t ulong = longitude * hgt->sideLength;
        uint16_t ulat = latitude * hgt->sideLength;

        // printf("%d %d ", ulong, ulat);
        return hgtReadElevationRaw(hgt, ulong * hgt->sideLength + ulat);
    }
    else // real world coordinates
    {
        // fixme: how should we handle this?
        return 0;
    }
}
