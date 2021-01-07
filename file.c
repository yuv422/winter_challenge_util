//
// Created by Eric Fry on 7/1/21.
//

#include <stdlib.h>
#include "file.h"

uint8 file_read1(FILE *file) {
    return (uint8)fgetc(file);
}

uint16 file_read2(FILE *file) {
    uint8 b0, b1;

    b0 = (uint8)fgetc(file);
    b1 = (uint8)fgetc(file);

    return (b0 + (b1<<8));
}

uint32 file_read4(FILE *file) {
    uint8 b0, b1, b2, b3;

    b0 = (uint8)fgetc(file);
    b1 = (uint8)fgetc(file);
    b2 = (uint8)fgetc(file);
    b3 = (uint8)fgetc(file);

    return (b0 + (b1<<8) + (b2<<16) + (b3<<24));
}

uint32 file_read_to_buffer(FILE *file, unsigned char *buf, uint32 num_bytes) {
    fread(buf,1,num_bytes,file); // FIX for partial read.
    return num_bytes;
}


void file_write2(uint16 word, FILE *file)
{
    fputc((uint8)(word & 0xff), file);
    fputc((uint8)((word >> 8) & 0xff), file);
}

void file_write4(uint32 double_word, FILE *file)
{
    fputc((uint8)(double_word & 0xff), file);
    fputc((uint8)((double_word >> 8) & 0xff), file);
    fputc((uint8)((double_word >> 16) & 0xff), file);
    fputc((uint8)((double_word >> 24) & 0xff), file);
}

uint32 file_get_size(char *filename) {
    FILE *fp = file_open(filename, "r");
    fseek(fp, 0, SEEK_END);
    uint32 size = ftell(fp);
    fclose(fp);
    return size;
}

FILE *file_open(char *filename, char *mode) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open %d with mode: %s", filename, mode);
        exit(1);
    }
    return fp;
}
