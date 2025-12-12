#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "/pub/FISE_OSSE11/syscall/lire.h"

int main() {
    lire();
    lire();

    // Exécution du programme lire
    execl("./lire", "lire", (char *)NULL);

    // Si execl échoue :
    fprintf(stderr, "Erreur execl : %s\n", strerror(errno));
    return 1;
}

//compilation : gcc -o lire3exec /pub/FISE_OSSE11/syscall/lire.c lire3exec.c
// test: ./lire3exec < /pub/FISE_OSSE11/syscall/data.in > data.out