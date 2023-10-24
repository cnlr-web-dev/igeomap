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
size_t factor = 3; // nu e chiar un scalar, e pentru trunchiere
size_t latura;

void genereazaSTL()
{
    int nrTriunghiuri = (latura / factor) * (latura / factor);                   // aria noului patrat
    stl_triangle_t *triunghiuri = calloc(nrTriunghiuri, sizeof(stl_triangle_t)); // alocam dinamic triunghiurile
    stl_header_t *antet = calloc(1, sizeof(stl_header_t));                       // alocam dinamic antetul

    // setam numarul de triunghiuri in antet
    antet->triunghiuri = nrTriunghiuri;

    // generam suprafata
    for (int x = factor; x < latura; x += factor)
    {
        for (int y = factor; y < latura; y += factor)
        {
            int xx = x / factor, yy = y / factor;
            stl_triangle_t *triunghi = &triunghiuri[yy * (latura / factor - 1) + xx];
            triunghi->normal.x = triunghi->normal.y = triunghi->normal.z = 0;

            triunghi->varf[0].x = xx + 1;
            triunghi->varf[0].y = yy + 1;
            triunghi->varf[0].z = suprafata[xx + 1][yy + 1];

            triunghi->varf[1].x = xx;
            triunghi->varf[1].y = yy;
            triunghi->varf[1].z = suprafata[xx][yy];

            triunghi->varf[2].x = xx - 1;
            triunghi->varf[2].y = yy - 1;
            triunghi->varf[2].z = suprafata[xx - 1][yy - 1];
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
    latura = hgt->sideLength;                  // latura impartita cu factor

    // cautam extremele elevatiei in tot fisierul si integram in matrice
    for (int x = 0; x < hgt->sideLength; x++)
    {
        for (int y = 0; y < hgt->sideLength; y++)
        {
            int16_t elevatie = hgtReadElevationRaw(hgt, y * hgt->sideLength + x);
            if (elevatie > max)
                max = elevatie;
            if (elevatie < min)
                min = elevatie;

            avg += abs(elevatie);

            suprafata[x][y] = elevatie;
        }
    }

    avg /= latura * latura; // facem media tuturor punctelor
    printf("%s: %dx%d min: %dm max: %dm avg: %ldm\n", hgt->filename, hgt->sideLength, hgt->sideLength, min, max, avg);

    genereazaSTL(); // fa generarea

    // deallocam memoria
    hgtDeleteContext(hgt);

    return 0;
}