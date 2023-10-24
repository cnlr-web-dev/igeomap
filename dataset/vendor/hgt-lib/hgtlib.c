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

    // swap endianness on little-endian systems
    // fixme: do detection
    size_t bufferIntegers = hgt->bufferSize / sizeof(int16_t);
    for (size_t i = 0; i < bufferIntegers; i++)
        hgt->buffer[i] = (hgt->buffer[i] << 8) | (hgt->buffer[i] >> 8);

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
