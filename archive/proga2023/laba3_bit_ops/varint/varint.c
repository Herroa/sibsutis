#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

/*
 * Диапазон             Вероятность
 * -------------------- -----------
 * [0; 128)             90%
 * [128; 16384)         5%
 * [16384; 2097152)     4%
 * [2097152; 268435455) 1%
 */
uint32_t generate_number()
{
    const int r = rand();
    const int p = r % 100;
    if (p < 90) {
        return r % 128;
    }
    if (p < 95) {
        return r % 16384;
    }
    if (p < 99) {
        return r % 2097152;
    }
    return r % 268435455;
}

void print_bin(unsigned char x){
    for(int i=0;i<8;i++){
        printf("%d", (x >> (7-i)) & 1);
    }
    printf("\n");
}

size_t encode_varint(uint32_t value, uint8_t* buf)
{
    assert(buf != NULL);
    uint8_t* cur = buf;
    while (value >= 0x80) {
        const uint8_t byte = (value & 0x7f) | 0x80;
        *cur = byte;
        value >>= 7;
        ++cur;
    }
    *cur = value;
    ++cur;
    return cur - buf;
}

uint32_t decode_varint(const uint8_t** bufp)
{
    const uint8_t* cur = *bufp;
    uint8_t byte = *cur++;
    uint32_t value = byte & 0x7f;
    size_t shift = 7;
    while (byte >= 0x80) {
        byte = *cur++;
        value += (byte & 0x7f) << shift;
        shift += 7;
    }
    *bufp = cur;
    return value;
}

size_t file_size(FILE *file)
{
    size_t size = 0;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

int main()
{
    FILE *uncompressed = fopen("uncompressed.dat", "wb");
    FILE *compressed = fopen("compressed.dat", "wb");

    for (int i = 0; i < 1000; i++) {
        uint8_t buf[4] = {0};
        uint32_t num = generate_number();
        size_t e_size = encode_varint(num, buf);
        fwrite(&num, 4, 1, uncompressed);
        fwrite(buf, 1, e_size, compressed);
    }

    fclose(compressed);
    fclose(uncompressed);

    uncompressed = fopen("uncompressed.dat", "rb");
    compressed = fopen("compressed.dat", "rb");
    uint32_t num;
    size_t compressed_size = file_size(compressed); 
    uint8_t *buf = malloc(sizeof(uint8_t) * compressed_size);
    uint8_t *buf_free = buf;

    for (size_t i = 0; i < compressed_size; i++) {
        fread(buf + i, 1, 1, compressed);
    }

    while (fread(&num, sizeof(int), 1, uncompressed)) {
        assert(num == decode_varint((const uint8_t **)&buf) && "Error: num != decode");
    }

    double com_ratio = (double)file_size(compressed) / (double)file_size(uncompressed);
    printf("compression ratio - %lf\n", com_ratio);

    fclose(compressed);
    fclose(uncompressed);
    free(buf_free);
}