
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1024

const char PUSH[]	= "PUSH";
const char POP[]	= "POP";
const char ADD[]	= "ADD";
const char SUB[]	= "SUB";
const char HALT[]	= "HALT";

const char DELIM[]	= " \t\n";

const char FILENAME[]	= "obj";

char buf[SIZE];
uint16_t bytes[SIZE];

int main(int argc, char *argv[]) {
    size_t len;
    uint16_t *p, x;
    FILE *fd;

    if (argc < 2) {
        puts("usage: ./asm <filename>");
        return EXIT_SUCCESS;
    }

    if ((fd = fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "error opening file [%s]\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (fseek(fd, 0L, SEEK_END) == -1) {
        fprintf(stderr, "file operation failed [%s]\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if ((len = ftell(fd)) == -1) {
        fprintf(stderr, "file operation failed [%s]\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    
    if (fseek(fd, 0L, SEEK_SET) == -1) {
        fprintf(stderr, "file operation failed [%s]\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (len > SIZE) {
        printf("input file [%s] is too large\n", argv[1]);
        exit(EXIT_SUCCESS);
    }

    if (fread(buf, 1, len, fd) != len) {
        fprintf(stderr, "error reading file [%s]\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (fclose(fd) == EOF) {
        fprintf(stderr, "error closing file [%s]\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    p = bytes;

    for (char *token = strtok(buf, DELIM); token; token = strtok(NULL, DELIM)) {
        if (!strncmp(PUSH, token, strlen(PUSH))) {
            *p = (uint16_t) 0x100;

            if ((token = strtok(NULL, DELIM)) == NULL) {
                fprintf(stderr, "invalid operand for operation\n");
                exit(EXIT_FAILURE);
            }

            if (strlen(token) > 2) {
                fprintf(stderr, "invalid operand length\n");
                exit(EXIT_FAILURE);
            }

            if (strlen(token) == 1) {
                x = token[0];
                x &= 0xf;
            }
            else {
                x = (token[0] << 8) | token[1];
                x &= 0x0f0f;
                x = (x >> 8) * 10 + (x & 0xff);

                if (x > 99) {
                    fprintf(stderr, "operand size out of range\n");
                    exit(EXIT_FAILURE);
                }
            }

            *p |= (x & 0xff);

            ++p;
        }
        else if (!strncmp(POP, token, strlen(POP))) {
            *p = (uint16_t) 0x200;
            ++p;
        }
        else if (!strncmp(ADD, token, strlen(ADD))) {
            *p = (uint16_t) 0x300;
            ++p;
        }
        else if (!strncmp(SUB, token, strlen(SUB))) {
            *p = (uint16_t) 0x400;
            ++p;
        }
        else if (!strncmp(HALT, token, strlen(HALT))) {
            *p = (uint16_t) 0x500;
            ++p;
        }
        else {
            fprintf(stderr, "invalid instruction [%s]\n", token);
            exit(EXIT_FAILURE);
        }
    }

    if ((fd = fopen(FILENAME, "wb")) == NULL) {
        fprintf(stderr, "error opening file [%s]\n", FILENAME);
        exit(EXIT_FAILURE);
    }

    if (fwrite(bytes, sizeof(uint16_t), p - bytes, fd) != p - bytes) {
        fprintf(stderr, "error writing file [%s]\n", FILENAME);
        exit(EXIT_FAILURE);
    }

    if (fclose(fd) == EOF) {
        fprintf(stderr, "error closing file [%s]\n", FILENAME);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

