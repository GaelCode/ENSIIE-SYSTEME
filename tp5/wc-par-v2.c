#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

// Faire ex 3.6 ==> interessant

int count_line(char* file, int debut, int fin) {
    FILE* f = fopen(file, "r");
    if (f == NULL) {
        perror("Erreur ouverture fichier");
        return 0;
    }

    if (fseek(f, debut, SEEK_SET) == -1) {
        perror("Erreur déplacement fichier");
        fclose(f);
        return 0;
    }

    int size = fin - debut;
    char* buf = malloc(size);
    size_t lu = fread(buf, 1, size, f);

    int nb = 0;
    for (size_t i = 0; i < lu; i++) {
        if (buf[i] == '\n')
            nb++;
    }

    free(buf);
    fclose(f);
    return nb;
}

int main(int argc, char* argv[]) {

    FILE* f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur ouverture (lecture seule) : %s\n", strerror(errno));
        return 1;
    }

    struct stat infos;
    if (fstat(fileno(f), &infos) == -1) {
        fprintf(stderr, "Erreur fstat : %s\n", strerror(errno));
        fclose(f);
        return 1;
    }

    off_t taille = infos.st_size;

    for (int i = 1; i <= 4; i++) {

        int pid = fork();
        if (pid == -1) {
            perror("Erreur fork");
            return 1;
        }

        if (pid == 0) {
            int nb = count_line(argv[1], taille*(i-1)/4, taille*i/4);
            exit(nb);
        }
    }
    int tailleFichier = 0;
    for (int i = 0; i < 4; i++){
        int status;
        wait(&status);
        tailleFichier += WEXITSTATUS(status);
        
    }
    printf("Nombre de sauts de ligne du fichier %s : %d\n", argv[1], tailleFichier);
    fclose(f);
    return 0;
}