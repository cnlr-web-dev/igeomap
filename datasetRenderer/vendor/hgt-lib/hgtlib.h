#include <stdint.h>

#define HGT_DATA_VOID -32768 // when no data is present for a point, this value is used

typedef struct
{
    const char *filename; // original file name
    FILE *fd;             // internal file descriptor
    uint16_t *buffer;     // internal buffer to the file contents
    uint32_t bufferSize;  // size of the buffer
    uint16_t sideLength;  // length in points of the side of the perimeter
} hgt_file_t;

hgt_file_t *hgtCreateContext(const char *filename);
void hgtDeleteContext(hgt_file_t *hgt);
int16_t hgtReadElevationRaw(hgt_file_t *hgt, size_t offset);
int16_t hgtReadElevation(hgt_file_t *hgt, double longitude, double latitude);