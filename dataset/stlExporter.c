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
size_t factor = 1; // nu e chiar un scalar, e pentru trunchiere
size_t latura;

vector3_t calculeazaNormala(vector3_t a, vector3_t b, vector3_t c)
{
    // https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
    vector3_t u = {.x = b.x - a.x,
                   .y = b.y - a.y,
                   .z = b.z - a.z};

    vector3_t v = {
        .x = c.x - a.x,
        .y = c.y - a.y,
        .z = c.z - a.z};

    vector3_t normala;
    normala.x = u.y * v.z - u.z * v.y;
    normala.y = u.z * v.x - u.x * v.z;
    normala.z = u.x * v.y - u.y * v.x;

    return normala;
}

void genereazaSTL()
{
    int nrTriunghiuri = (latura / factor) * (latura / factor);                   // aria noului patrat
    stl_triangle_t *triunghiuri = calloc(nrTriunghiuri, sizeof(stl_triangle_t)); // alocam dinamic triunghiurile
    stl_header_t *antet = calloc(1, sizeof(stl_header_t));                       // alocam dinamic antetul

    // setam numarul de triunghiuri in antet
    antet->triunghiuri = 1;

    // generam suprafata
    uint32_t i = 0;
    for (int x = factor; x < latura; x += factor * 2)
    {
        for (int y = factor; y < latura; y += factor * 2)
        {
            int xx1 = x / 2, yy1 = y / 2;
            int xx2 = xx1 + 1, yy2 = yy1 + 1;
            stl_triangle_t *triunghi1 = &triunghiuri[i++];
            stl_triangle_t *triunghi2 = &triunghiuri[i++];

            // creaza un patrat din 2 triunghiuri
            triunghi1->varf[0].x = xx1;
            triunghi1->varf[0].y = yy1;

            triunghi1->varf[1].x = xx1;
            triunghi1->varf[1].y = yy2;

            triunghi1->varf[2].x = xx2;
            triunghi1->varf[2].y = yy2;

            triunghi2->varf[0].x = xx2;
            triunghi2->varf[0].y = yy2;

            triunghi2->varf[1].x = xx2;
            triunghi2->varf[1].y = yy1;

            triunghi2->varf[2].x = xx1;
            triunghi2->varf[2].y = yy1;

            // umplem adancimea in functie de coordonate
            triunghi1->varf[0].z = suprafata[(int)triunghi1->varf[0].x][(int)triunghi1->varf[0].y];
            triunghi1->varf[1].z = suprafata[(int)triunghi1->varf[1].x][(int)triunghi1->varf[1].y];
            triunghi1->varf[2].z = suprafata[(int)triunghi1->varf[2].x][(int)triunghi1->varf[2].y];

            triunghi2->varf[0].z = suprafata[(int)triunghi2->varf[0].x][(int)triunghi2->varf[0].y];
            triunghi2->varf[1].z = suprafata[(int)triunghi2->varf[1].x][(int)triunghi2->varf[1].y];
            triunghi2->varf[2].z = suprafata[(int)triunghi2->varf[2].x][(int)triunghi2->varf[2].y];

            triunghi1->normal = calculeazaNormala(triunghi1->varf[0], triunghi1->varf[1], triunghi1->varf[2]);
            triunghi2->normal = calculeazaNormala(triunghi2->varf[0], triunghi2->varf[1], triunghi2->varf[2]);
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
    hgt = hgtCreateContext("raw/N45E024.hgt"); // creeaza context pentru fisierul hgt
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

    //

    avg /= latura * latura; // facem media tuturor punctelor
    printf("%s: %dx%d min: %dm max: %dm avg: %ldm\n", hgt->filename, hgt->sideLength, hgt->sideLength, min, max, avg);

    genereazaSTL(); // fa generarea

    // deallocam memoria
    hgtDeleteContext(hgt);

    return 0;
}