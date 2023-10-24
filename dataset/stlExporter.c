#include <stdio.h>
#include <stdlib.h>
#include <hgt-lib/hgtlib.h>
#include <stdint.h>
#include <assert.h>
#include <memory.h>

typedef _Float32 float32_t;

typedef struct __attribute__((packed))
{
    float32_t x, y, z;
} vector3_t;

typedef struct __attribute__((packed))
{
    // antetul standard stl
    uint8_t ignorat[80];  // ignorat de programe
    uint32_t triunghiuri; // numarul de triunghiuri in fisier
} stl_header_t;

typedef struct __attribute__((packed))
{
    vector3_t normal;  // vector orientat in afara obiectului solid
    vector3_t varf[3]; // varfurile triunghiului
    uint16_t atribute; // bytes folositi drept atribute
} stl_triangle_t;

// fisierul hgt deschis
hgt_file_t *hgt;

// extremele elevatiei
int16_t max = HGT_DATA_VOID, min = INT16_MAX;
uint64_t avg = 0;

void genereazaSTL()
{
    const int TRIUNGHIURI = 1000;                                              // un triunghi (de moment)
    stl_triangle_t *triunghiuri = calloc(TRIUNGHIURI, sizeof(stl_triangle_t)); // alocam dinamic triunghiurile
    stl_header_t *antet = calloc(1, sizeof(stl_header_t));                     // alocam dinamic antetul

    // setam numarul de triunghiuri in antet
    antet->triunghiuri = TRIUNGHIURI;

    const float32_t apropiere = 0.001;
    for (float32_t i = 0; i < TRIUNGHIURI; i++)
    {
        stl_triangle_t *triunghi = &triunghiuri[(int)i];
        triunghi->normal.x = triunghi->normal.y = triunghi->normal.z = -1;

        float32_t punctX = i * apropiere, punctY = i * apropiere;

        triunghi->varf[0].x = 0;
        triunghi->varf[0].y = 0;

        triunghi->varf[1].x = punctX;
        triunghi->varf[1].y = punctY;

        triunghi->varf[2].x = 1;
        triunghi->varf[2].y = 1;

        triunghi->varf[0].z = 0;
        triunghi->varf[1].z = i;
        triunghi->varf[2].z = 0;
    }

    // scrie in fisier
    FILE *fd = fopen("test.stl", "wb");
    fwrite(antet, sizeof(stl_header_t), 1, fd);                   // scrie antetul
    fwrite(triunghiuri, sizeof(stl_triangle_t), TRIUNGHIURI, fd); // scrie triunghiurile
    fclose(fd);
}

int main()
{
    hgt = hgtCreateContext("raw/N48E029.hgt");       // creeaza context pentru fisierul hgt
    size_t arie = hgt->sideLength * hgt->sideLength; // aria perimetrului descris

    // cautam extremele elevatiei in tot fisierul
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

    genereazaSTL(); // fa generarea

    // deallocam memoria
    hgtDeleteContext(hgt);

    return 0;
}