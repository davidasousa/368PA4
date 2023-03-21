#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "hbt.h"

typedef unsigned char uchar;

static void bit_reader(FILE* fp)
{
    fseek(fp, 0L, SEEK_END);
    int length = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    char* file_contents = malloc(sizeof(*file_contents) * length);

    fread(file_contents, sizeof(char), length, fp);
    // check error
    
    printf("%s", file_contents);

    for(int idx = 0; idx < length; idx++)
    {
        switch(file_contents[idx])
        {

        }
    }

    return;
}

int main(int argc, char* argv[])
{
    printf("Hello World\n\n");

    FILE* fp = fopen(argv[1], "r");
    bit_reader(fp);
    fclose(fp);
    printf("\n");

    return EXIT_SUCCESS;
}
