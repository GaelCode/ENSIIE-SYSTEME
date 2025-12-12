#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#include "/pub/FISE_OSSE11/syscall/lire.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s infile outfile\n", argv[0]);
        return 1;
    }

    // Premier appel avec stdin/stdout normaux
    lire();

    int fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1) {
        fprintf(stderr, "%s: cannot open '%s' for reading: %s\n", argv[0], argv[1], strerror(errno));
        return 1;
    }

    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC);
    if (fd_out == -1) {
        fprintf(stderr, "%s: cannot open '%s' for writing: %s\n", argv[0], argv[2], strerror(errno));
        close(fd_in);
        return 1;
    }

    dup2(fd_in, 0);
    dup2(fd_out, 1);
    close(fd_in);
    close(fd_out);

    lire();

    execl("./lire", "lire", (char *)NULL);
    fprintf(stderr, "Erreur execl : %s\n", strerror(errno));
    return 1;
}