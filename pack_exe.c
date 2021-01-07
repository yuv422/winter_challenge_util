//
// Created by Eric Fry on 7/1/21.
//

#include <strings.h>
#include "file.h"

uint32 getEXESize(FILE *file) {
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
    return offset;
}

void encodeFilename(unsigned char *filename) {
    for(int i = 0; i < 14; i++) {
        if (filename[i] != 0) {
            filename[i] = filename[i] + 0x60;
        }
    }
}

void writeFileToArchive(char *filename, FILE *outfile) {
    FILE *inputFile = file_open(filename, "r");
    uint32 size = file_get_size(filename);
    for (int i = 0; i < size; i++) {
        uint8 byte = file_read1(inputFile);
        fwrite(&byte, 1, 1, outfile);
    }
    fclose(inputFile);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s originalEXEFile outputFile [files to be packed]\n", argv[0]);
        return 1;
    }

    FILE *inputFile = file_open(argv[1], "r");
    FILE *outputFile = file_open(argv[2], "wb");

    uint32 exeSize = getEXESize(inputFile);
    uint16 numEntries = argc - 3;

    printf("Original EXE Size: %d\n", exeSize);
    printf("numEntries: %d\n", numEntries);

    fseek(inputFile, 0, SEEK_SET);

    if (exeSize > 0) {
        for (int i = 0; i < exeSize; i++) {
            uint8 byte = file_read1(inputFile);
            fwrite(&byte, 1, 1, outputFile);
        }
    }

    file_write2(0x424d, outputFile);
    file_write2(numEntries, outputFile);
    uint32 headerTableOffset = ftell(outputFile);

    uint32 dataOffset = headerTableOffset + (numEntries * 22);
    for( int i = 0; i < numEntries; i++) {
        unsigned char filenameEncoded[14];
        strcpy((char *)filenameEncoded, argv[3 + i]);
        uint32 size = file_get_size((char *)filenameEncoded);
        file_write4(size, outputFile);
        file_write4(dataOffset, outputFile);
        encodeFilename(filenameEncoded);
        fwrite(filenameEncoded, 1, 14, outputFile);
        dataOffset += size;
    }

    //TODO write actual filedata here

    for( int i = 0; i < numEntries; i++) {
        writeFileToArchive(argv[3 + i], outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
