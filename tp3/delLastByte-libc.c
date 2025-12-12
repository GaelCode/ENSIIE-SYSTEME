#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage : %s [cheminFichier]\n", argv[0]);
        return 1;
    }

    // a. ouvrir le fichier f en mode lecture seule
    FILE* f = fopen(argv[1], "rb");  // mode binaire pour éviter les surprises
    if (!f) {
        fprintf(stderr, "Erreur ouverture (lecture seule) : %s\n", strerror(errno));
        return 1;
    }

    // b. calculer la taille du fichier avec fseek + ftell
    if (fseek(f, 0, SEEK_END) != 0) {
        fprintf(stderr, "Erreur fseek : %s\n", strerror(errno));
        fclose(f);
        return 1;
    }

    long taille = ftell(f);
    if (taille == -1L) {
        fprintf(stderr, "Erreur ftell : %s\n", strerror(errno));
        fclose(f);
        return 1;
    }

    // si fichier vide ou d’un seul octet
    if (taille <= 1) {
        fprintf(stderr, "Le fichier est vide ou ne contient qu’un seul octet, rien à retirer.\n");
        fclose(f);
        return 0;
    }

    // c. allouer un tableau de taille t−1
    char* buffer = malloc(taille - 1);
    if (!buffer) {
        fprintf(stderr, "Erreur d’allocation mémoire.\n");
        fclose(f);
        return 1;
    }

    // repositionner au début du fichier
    rewind(f);

    // d. lire d’un seul bloc les t−1 premiers octets
    size_t lus = fread(buffer, sizeof(char), taille - 1, f);
    if (lus < (size_t)(taille - 1)) {
        if (ferror(f)) {
            fprintf(stderr, "Erreur lecture : %s\n", strerror(errno));
            fclose(f);
            free(buffer);
            return 1;
        }
    }

    // e. fermer f et le réouvrir en mode écriture seule + troncature
    if (fclose(f) == EOF) {
        fprintf(stderr, "Erreur fermeture (lecture) : %s\n", strerror(errno));
        free(buffer);
        return 1;
    }

    FILE* fw = fopen(argv[1], "wb");  // écriture binaire avec troncature
    if (!fw) {
        fprintf(stderr, "Erreur réouverture (écriture seule) : %s\n", strerror(errno));
        free(buffer);
        return 1;
    }

    // f. écrire le contenu du tableau dans f
    size_t ecrits = fwrite(buffer, sizeof(char), taille - 1, fw);
    if (ecrits < (size_t)(taille - 1)) {
        fprintf(stderr, "Erreur écriture : %s\n", strerror(errno));
        fclose(fw);
        free(buffer);
        return 1;
    }

    // g. fermer le fichier et libérer la mémoire
    if (fclose(fw) == EOF) {
        fprintf(stderr, "Erreur fermeture (écriture) : %s\n", strerror(errno));
        free(buffer);
        return 1;
    }

    free(buffer);
    printf("Dernier octet supprimé avec succès (version haut niveau).\n");

    return 0;
}