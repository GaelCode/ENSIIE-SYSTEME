#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // Chemin de l'exécutable cible
    const char *exePath = "/pub/FISE_OSSE11/shell/iacmp/iacmp";

    // Définir les variables d'environnement nécessaires
    if (setenv("IacmpDir", "/pub/FISE_OSSE11/shell/iacmp", 1) == -1) {
        fprintf(stderr, "Erreur setenv(IacmpDir): %s\n", strerror(errno));
        return 1;
    }

    if (setenv("LD_LIBRARY_PATH", "/pub/FISE_OSSE11/shell/iacmp/lib", 1) == -1) {
        fprintf(stderr, "Erreur setenv(LD_LIBRARY_PATH): %s\n", strerror(errno));
        return 1;
    }

    // Construire un tableau d'arguments pour execv
    // => on garde argv[0] comme nom du programme
    char **new_argv = malloc((argc + 1) * sizeof(char *));
    if (new_argv == NULL) {
        fprintf(stderr, "Erreur d’allocation mémoire.\n");
        return 1;
    }

    new_argv[0] = (char *)exePath; // premier argument = nom du programme exécuté
    for (int i = 1; i < argc; i++) {
        new_argv[i] = argv[i];
    }
    new_argv[argc] = NULL; // fin du tableau

    // Exécuter le programme iacmp original
    execv(exePath, new_argv);

    // Si execv échoue :
    fprintf(stderr, "Erreur execv(%s) : %s\n", exePath, strerror(errno));
    free(new_argv);
    return 1;
}

// compilation: gcc -o iacmp iacmp.c
// ./iacmp "chat roux" "chat blanc" "chat noir"