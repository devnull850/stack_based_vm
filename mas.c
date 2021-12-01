
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    char *filename;
    size_t len;
    pid_t pid;
    int stat;

    if (argc < 2) {
        puts("usage: ./mas <filename>");
        return EXIT_SUCCESS;
    }

    len = strlen(argv[1]);

    if ((filename = malloc(len + 1)) == NULL) {
        fprintf(stderr, "error allocating memory\n");
        exit(EXIT_FAILURE);
    }

    strncpy(filename, argv[1], len);

    if ((pid = fork()) == -1) {
        fprintf(stderr, "process creation failed\n");
        exit(EXIT_FAILURE);
    }

    if (!pid) { /* child */
        execl("asm", "asm", filename, (char *) 0);
    }
    else { /* parent */
        do {
            waitpid(pid, &stat, WUNTRACED | WCONTINUED);
        } while (!WIFEXITED(stat) && !WIFSIGNALED(stat));

    }

    if ((pid = fork()) == -1) {
        fprintf(stderr, "process creation failed\n");
        exit(EXIT_FAILURE);
    }

    if (!pid) { /* child */
        execl("vm", "vm", "obj", (char *) 0);
    }
    else { /* parent */
        do {
            waitpid(pid, &stat, WUNTRACED | WCONTINUED);
        } while (!WIFEXITED(stat) && !WIFSIGNALED(stat));
    }

    free(filename);

    if (remove("obj") == -1) {
        fprintf(stderr, "error deleting object file\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

