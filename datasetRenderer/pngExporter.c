#include <stdio.h>
#include <stdlib.h>
#include <hgt-lib/hgtlib.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

// din arduino
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main()
{
    hgt_file_t *hgt = hgtCreateContext("dataset/N48E029.hgt");

    // cautam elevatia minima in tot fisierul
    int32_t max = HGT_DATA_VOID, min = INT16_MAX, avg = 0;
    for (uint32_t i = 0; i < lines * samples; i++)
    {
        int16_t elevatie = hgtReadElevationRaw(hgt, i);
        if (elevatie > max)
            max = elevatie;
        if (elevatie < min)
            min = elevatie;
    }
    avg = (max + min) / 2;
    printf("%s: min: %dm max: %dm avg: %dm\n", hgt->filename, min, max, avg);

    // pregatim datele imaginii
    int index = 0;
    uint8_t *buffer = malloc(lines * samples * 3); // intensitate
    for (uint32_t i = 0; i < lines * samples * 3; i++)
    {
        int16_t elevatie = hgtReadElevationRaw(hgt, i);
        buffer[index++] = (uint8_t)map(elevatie, min, max, 0, 255);
    }

    // o scriem
    stbi_write_png_compression_level = 0;
    stbi_write_bmp("test.bmp", lines * 3 - 2, samples * 3, 1, buffer);

    hgtDeleteContext(hgt);

    return 0;
}