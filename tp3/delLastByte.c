#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s [cheminFichier]\n", argv[0]);
        return 1;
    }

    // a. ouvrir le fichier f en mode lecture seule
    int f = open(argv[1], O_RDONLY);
    if (f == -1) {
        fprintf(stderr, "Erreur ouverture (lecture seule) : %s\n", strerror(errno));
        return 1;
    }

    // b. calculer la taille t du fichier f avec lseek
    off_t taille = lseek(f, 0, SEEK_END);
    if (taille == -1) {
        fprintf(stderr, "Erreur fseek : %s\n", strerror(errno));
        close(f);
        return 1;
    }

    // si fichier vide ou d’un seul octet
    if (taille <= 1) {
        fprintf(stderr, "Le fichier est vide ou ne contient qu’un seul octet, rien à retirer.\n");
        close(f);
        return 0;
    }

    // c. allouer un tableau de taille t−1
    char* buffer = malloc((taille - 1) * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Erreur d’allocation mémoire.\n");
        close(f);
        return 1;
    }

    // repositionner le curseur au début du fichier
    if (fseek(f, 0, SEEK_SET) == -1) {
        fprintf(stderr, "Erreur repositionnement : %s\n", strerror(errno));
        close(f);
        free(buffer);
        return 1;
    }

    // d. lire d’un seul bloc les t−1 premiers octets de f
    ssize_t nbytes = read(f, buffer, taille - 1);
    if (nbytes == -1) {
        fprintf(stderr, "Erreur lecture : %s\n", strerror(errno));
        close(f);
        free(buffer);
        return 1;
    }

    // e. fermer f et le réouvrir en mode écriture seule + troncature
    if (close(f) == -1) {
        fprintf(stderr, "Erreur fermeture : %s\n", strerror(errno));
        free(buffer);
        return 1;
    }

    int fw = open(argv[1], O_WRONLY | O_TRUNC);
    if (fw == -1) {
        fprintf(stderr, "Erreur réouverture (écriture seule) : %s\n", strerror(errno));
        free(buffer);
        return 1;
    }

    // f. écrire le contenu du tableau dans f
    ssize_t wbytes = write(fw, buffer, taille - 1);
    if (wbytes == -1) {
        fprintf(stderr, "Erreur écriture : %s\n", strerror(errno));
        close(fw);
        free(buffer);
        return 1;
    }

    // g. fermer le fichier et libérer la mémoire
    if (close(fw) == -1) {
        fprintf(stderr, "Erreur fermeture : %s\n", strerror(errno));
        free(buffer);
        return 1;
    }

    free(buffer);
    return 0;
}