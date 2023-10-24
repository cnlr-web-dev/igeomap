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

// suprafata terenului
int16_t suprafata[4000][4000];
size_t arie;
size_t factor = 3;

void genereazaSTL()
{
    int latura = hgt->sideLength / factor;                                       // latura impartita cu factor
    int nrTriunghiuri = latura * latura;                                         // aria noului patrat
    stl_triangle_t *triunghiuri = calloc(nrTriunghiuri, sizeof(stl_triangle_t)); // alocam dinamic triunghiurile
    stl_header_t *antet = calloc(1, sizeof(stl_header_t));                       // alocam dinamic antetul

    // setam numarul de triunghiuri in antet
    antet->triunghiuri = nrTriunghiuri;

    // generam suprafata
    for (int x = 1; x < latura; x++)
    {
        for (int y = 1; y < latura; y++)
        {
            stl_triangle_t *triunghi = &triunghiuri[y * latura + x];
            triunghi->normal.x = triunghi->normal.y = triunghi->normal.z = 0;

            triunghi->varf[0].x = x + 1;
            triunghi->varf[0].y = y + 1;
            triunghi->varf[0].z = suprafata[x + 1][y + 1];

            triunghi->varf[1].x = x;
            triunghi->varf[1].y = y;
            triunghi->varf[1].z = suprafata[x][y];

            triunghi->varf[2].x = x - 1;
            triunghi->varf[2].y = y - 1;
            triunghi->varf[2].z = suprafata[x - 1][y - 1];
        }
    }

    // scrie in fisier
    FILE *fd = fopen("test.stl", "wb");
    fwrite(antet, sizeof(stl_header_t), 1, fd);                     // scrie antetul
    fwrite(triunghiuri, sizeof(stl_triangle_t), nrTriunghiuri, fd); // scrie triunghiurile
    fclose(fd);
}

int main()
{
    hgt = hgtCreateContext("raw/N48E029.hgt"); // creeaza context pentru fisierul hgt
    arie = hgt->sideLength * hgt->sideLength;  // aria perimetrului descris

    // cautam extremele elevatiei in tot fisierul si integram in matrice
    for (size_t i = 0; i < arie; i += factor)
    {
        int16_t elevatie = hgtReadElevationRaw(hgt, i);
        if (elevatie > max)
            max = elevatie;
        if (elevatie < min)
            min = elevatie;

        avg += abs(elevatie);

        suprafata[(i / factor) / arie][(i / factor) % arie] = elevatie;
    }
    avg /= arie; // facem media tuturor punctelor
    printf("%s: %dx%d min: %dm max: %dm avg: %ldm\n", hgt->filename, hgt->sideLength, hgt->sideLength, min, max, avg);

    genereazaSTL(); // fa generarea

    // deallocam memoria
    hgtDeleteContext(hgt);

    return 0;
}