//
// Created by Eric Fry on 7/1/21.
//

#ifndef WINTER_CHALLENGE_UTIL_FILE_H
#define WINTER_CHALLENGE_UTIL_FILE_H

#include <stdio.h>
#include "defs.h"

FILE *file_open(char *filename, char *mode);
uint8 file_read1(FILE *file);
uint16 file_read2(FILE *file);
uint32 file_read4(FILE *file);
uint32 file_read_to_buffer(FILE *file, unsigned char *buf, uint32 num_bytes);

void file_write2(uint16 word, FILE *file);
void file_write4(uint32 double_word, FILE *file);

uint32 file_get_size(char *filename);

#endif //WINTER_CHALLENGE_UTIL_FILE_H
