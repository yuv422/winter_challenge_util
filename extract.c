#include <stdio.h>
#include <stdlib.h>

#include "file.h"

void dump_file(FILE *archive, uint32 size, uint32 offset, char *filename) {
    fseek(archive, offset, SEEK_SET);

    FILE *out = file_open(filename, "wb");
    for(int i = 0; i < size; i++) {
        uint8 byte = file_read1(archive);
        fwrite(&byte, 1, 1, out);
    }
    fclose(out);
}

uint32 find_offset(FILE *file) {
    uint16 word = file_read2(file);
    uint32 offset = 0;
    while(word == 0x5a4d) { // 'MZ'
        uint16 bytesInLastBlock = file_read2(file);
        uint16 blocksInEXE = file_read2(file);
        offset = blocksInEXE * 512;
        if (bytesInLastBlock) {
            offset -= (512 - bytesInLastBlock);
        }
        word = file_read2(file);
    }

    fseek(file, offset, SEEK_SET);

    uint16 magic = file_read2(file);

    if (magic != 0x424d) { // 'MB'
        printf("No valid 'MB' section found in file!\n");
        exit(1);
    }

    return offset;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s filename\n", argv[0]);
        exit(1);
    }

    FILE *file = file_open(argv[1], "r");

    uint32 offset = find_offset(file);

    fseek(file, offset + 2, SEEK_SET);
    uint16 numRecords = file_read2(file);

    printf("num records = %d\n", numRecords);

    for(int i = 0; i < numRecords; i++) {
        fseek(file, offset + 4 + (i * 22), SEEK_SET);
        char filename[14];
        uint32 size = file_read4(file);
        uint32 fileOffset = file_read4(file);
        file_read_to_buffer(file, (unsigned char *)filename, 14);

        for(int j = 0; j < 14; j++) {
            if (filename[j] != 0) {
                filename[j] -= 0x60;
            }
        }

        printf("file: %s - size: %d, offsetInArchive: 0x%x\n", filename, size, fileOffset);

        dump_file(file, size, fileOffset, filename);
    }
    fclose(file);
    return 0;
}
