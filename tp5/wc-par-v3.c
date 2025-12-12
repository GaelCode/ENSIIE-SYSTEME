#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

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
        fprintf(stderr, "Erreur ouverture (lecture seule) : %d\n", strerror(errno));
        return 1;
    }

    struct stat infos;
    if (fstat(fileno(f), &infos) == -1) {
        fprintf(stderr, "Erreur fstat : %d\n", strerror(errno));
        fclose(f);
        return 1;
    }

    int pipefd[2];
    if(pipe(pipefd)==-1) {
        printf("Erreur dans l'ouverture du pipe");
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
            close(pipefd[0]); // fermeture du pipe en lecture
            int nb = count_line(argv[1], taille*(i-1)/4, taille*i/4);
            if (write(pipefd[1], &nb, sizeof(nb)) == -1){
                exit(1);
            }
            close(pipefd[1]);
            exit(0);
        }
    }
    close(pipefd[1]);
    int tailleFichier = 0;
    for (int i = 0; i < 4; i++){
        int status;
        if (wait(&status)==-1) {
            printf("Erreur pas de fils");
            exit(1);
        } 
    }

    for (int i = 0; i < 4; i++) {
        int lignes_part = 0;
        ssize_t lu = read(pipefd[0], &lignes_part, sizeof(lignes_part));
        if (lu == -1) {
            perror("read");
        } else if (lu == 0) {
            fprintf(stderr, "pipe fermé avant d'avoir tout lu(j=%d)\n", i);
            break;
        }
        tailleFichier += lignes_part;
    }

    printf("Nombre de sauts de ligne du fichier %s : %d\n", argv[1], tailleFichier);
    fclose(f);
    return 0;
}