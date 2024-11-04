#include <stdio.h>
#include <string.h>
#include "coder.h"
#include "command.h"

int main(int argc, char *argv[])
{
    if(argc!= 4){
        printf("Not enough arguments!\n");
        return 0;
    }
    const char *command = argv[1];
    const char *in_file_name = argv[2];
    const char *out_file_name = argv[3];

    if(strcmp(command, "encode") == 0){
        encode_file(in_file_name, out_file_name);
    }else if(strcmp(command, "decode") == 0) {
		decode_file(in_file_name, out_file_name);
    }else{
        printf("Wrong command!\n");
        return -1;
    }


    return 0;
}