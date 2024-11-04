#include "coder.h"
#include "command.h"

int encode_file(const char *in_file_name, const char *out_file_name)
{
    FILE *in;
    FILE *out;
    in = fopen(in_file_name,"r");
    if(!in){
        printf("Input file not found\n");
        return -1;
    }
    out = fopen(out_file_name,"wb");
    if(!out){
        printf("Output file isn't open\n");
        return -1;
    }

    uint32_t code_point;
    while(!feof(in)){
        fscanf(in, "%" SCNx32, &code_point);
        printf("%" PRIx32 "\n", code_point);

        CodeUnit code_unit;
        if(encode(code_point, &code_unit) < 0){
            printf("Encode error\n ");
            return -1;
        }

        write_code_unit(out, &code_unit);
    }
    fclose(in);
    fclose(out);

    return 0;
}

int decode_file(const char *in_file_name, const char *out_file_name)
{
    FILE *in;
    FILE *out;
    in = fopen(in_file_name,"rb");
    if(!in){
        printf("Input file not found\n");
        return -1;
    }
    out = fopen(out_file_name,"w");
    if(!out){
        printf("Output file isn't open\n");
        return -1;
    }

    CodeUnit code_unit;
    uint32_t code_point;
    while (!feof(in)) {
        if (!read_next_code_unit(in, &code_unit)) {
            code_point = decode(&code_unit);
            if (!feof(in)) {
                fprintf(out, "%" SCNx32 "\n", code_point);
            }
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}