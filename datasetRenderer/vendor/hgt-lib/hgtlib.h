#include <stdint.h>

#define HGT_DATA_VOID -32768 // when no data is present for a point, this value is used

typedef struct
{
    const char *filename;
    FILE *fd;
    uint16_t *buffer;
    uint32_t bufferSize;
} hgt_file_t;

static const uint32_t lines = 1201, samples = 1201; // SRTM3

hgt_file_t *hgtCreateContext(const char *filename);
void hgtDeleteContext(hgt_file_t *hgt);
int16_t hgtReadElevationRaw(hgt_file_t *hgt, size_t offset);
int16_t hgtReadElevation(hgt_file_t *hgt, double longitude, double latitude);