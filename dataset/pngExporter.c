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
    hgt_file_t *hgt = hgtCreateContext("raw/N48E029.hgt");
    size_t arie = hgt->sideLength * hgt->sideLength; // aria patratului format de perimetru

    // cautam extremele elevatiei in tot fisierul
    int16_t max = HGT_DATA_VOID, min = INT16_MAX;
    uint64_t avg = 0;
    for (size_t i = 0; i < arie; i++)
    {
        int16_t elevatie = hgtReadElevationRaw(hgt, i);
        if (elevatie > max)
            max = elevatie;
        if (elevatie < min)
            min = elevatie;

        avg += abs(elevatie);
    }
    avg /= arie; // facem media tuturor punctelor
    printf("%s: min: %dm max: %dm avg: %ldm\n", hgt->filename, min, max, avg);

    // pregatim datele imaginii
    uint8_t *buffer = malloc(arie); // intensitatea este reprezentata de o singura valoare pe 8 biti
    for (size_t i = 0; i < arie; i++)
    {
        int16_t elevatie = hgtReadElevationRaw(hgt, i);
        buffer[i] = (uint8_t)map(elevatie, min, max, 0, 255);
    }

    // o scriem
    stbi_write_png_compression_level = 0;                                                                                                                    // dezactivam compresia
    stbi_write_png("test.png", hgt->sideLength, hgt->sideLength, 1, buffer, hgt->sideLength * sizeof(uint8_t) /*cam redundant, 8 biti reprezinta un byte*/); // generam fisierul .png

    // deallocam memoria
    hgtDeleteContext(hgt);
    free(buffer);

    return 0;
}