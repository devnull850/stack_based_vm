
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define SIZE 1024

enum {
    PUSH = 1,
    POP, 
    ADD,
    SUB,
    HALT
};

uint8_t stack[SIZE];
uint8_t code[SIZE];

int main(int argc, char *argv[]) {
    uint16_t *ip, instr;
    uint8_t *sp, op_code, x;
    FILE *fd;
    size_t len;

    if (argc < 2) {
        puts("usage: ./vm <filename>");
        return EXIT_SUCCESS;
    }

    if ((fd = fopen(argv[1], "rb")) == NULL) {
        fprintf(stderr, "error opening file [%s]\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (fseek(fd, 0, SEEK_END) == -1) {
        fprintf(stderr, "file operation failed [%s]\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if ((len = ftell(fd)) == -1) {
        fprintf(stderr, "file operation failed [%s]\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (fseek(fd, 0, SEEK_SET) == -1) {
        fprintf(stderr, "file operation failed [%s]\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (len > SIZE) {
        fprintf(stderr, "input file [%s] is too large\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (fread(code, 1, len, fd) != len) {
        fprintf(stderr, "error reading file [%s]\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    if (fclose(fd) == EOF) {
        fprintf(stderr, "error closing file [%s]\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    ip = (uint16_t *) code;
    sp = stack + SIZE;

    while (1) {
        instr = *ip;
        ++ip;

        op_code = (instr >> 8) & 0xff;

        switch (op_code) {
            case PUSH:
                if (sp == stack) {
                    fprintf(stderr, "invalid stack state for operation PUSH\n");
                    exit(EXIT_FAILURE);
                }

                --sp;
                *sp = (uint8_t) (instr & 0xff);
                break;
            case POP:
                if (sp == stack + SIZE) {
                    fprintf(stderr, "invalid stack state for operation POP\n");
                    exit(EXIT_FAILURE);
                }

                printf("%u\n", *sp);
                ++sp;
                break;
            case ADD:
                {
                    if (stack + SIZE - sp < 2) {
                        fprintf(stderr, "invalid stack state for operation ADD\n");
                        exit(EXIT_FAILURE);
                    }

                    x = *sp;
                    ++sp;
                    *sp += x;
                }

                break;
            case SUB:
                {
                    if (stack + SIZE - sp < 2) {
                        fprintf(stderr, "invalid stack state for operation SUB\n");
                        exit(EXIT_FAILURE);
                    }

                    x = *sp;
                    ++sp;
                    *sp -= x;
                }

                break;
            case HALT:
                return EXIT_SUCCESS;
                break;
            default:
                puts("bad op code");
                exit(EXIT_FAILURE);
        };
    }

    return EXIT_SUCCESS;
}

