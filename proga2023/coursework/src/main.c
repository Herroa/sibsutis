#include "mur.h"
#include "dir.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    char *template;
    char *folder;
    if (strcmp(argv[1], "-r") == 0)
    {
        if (argc < 4)
        {
            printf("Not enough arguments\nExample:\n ./bmatcher -r 'ABBA' .\n");
            return -1;
        }
        else
        {
            template = argv[2];
            folder = argv[3];
            next_dir(template, folder);
            //recursive
        }
    }
    else
    {
        if (argc < 3)
        {
            printf("Not enough arguments\nExample:\n ./bmatcher -r 'ABBA' .\n");
            return -1;
        }
        else
        {
            template = argv[1];
            folder = argv[2];
            this_dir(template, folder);
            //not recursive
        }
    }


    return 0;
}
