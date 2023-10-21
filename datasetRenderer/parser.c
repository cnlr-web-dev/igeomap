#include <stdio.h>
#include <stdlib.h>
#include <hgt-lib/hgtlib.h>
#include <stb_image_write.h>

int main()
{
    hgt_file_t *hgt = hgtCreateContext("dataset/N48E029.hgt"); // fixme: don't hard-code

    for (double i = 0; i < 1; i += 0.01)
        printf("%d ", hgtReadElevation(hgt, 0, i));

    hgtDeleteContext(hgt);

    return 0;
}