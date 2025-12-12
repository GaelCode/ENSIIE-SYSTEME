#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct arg {
    char* file;
    int begin;
    int end;
};

int resultat = 0;

void* count_line(void* a) {
    struct arg* args = (struct arg*) a;

    FILE* f = fopen(args->file, "r");
    if (!f) {
        perror("Erreur ouverture fichier");
        pthread_exit(NULL);
    }

    if (fseek(f, args->begin, SEEK_SET) == -1) {
        perror("Erreur déplacement fichier");
        fclose(f);
        pthread_exit(NULL);
    }

    int size = args->end - args->begin;
    char* buf = malloc(size);
    if (!buf) {
        perror("malloc");
        fclose(f);
        pthread_exit(NULL);
    }

    size_t lu = fread(buf, 1, size, f);

    int local = 0;
    for (size_t i = 0; i < lu; i++) {
        if (buf[i] == '\n')
            local++;
    }

    free(buf);
    fclose(f);

    pthread_mutex_lock(&mutex); // lock la donnée pour éviter un changement en même temps
    resultat += local;
    pthread_mutex_unlock(&mutex); // unlock de la donnée pour la rendre à nouveau modifiable

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror("Erreur ouverture");
        return 1;
    }

    struct stat infos;
    if (fstat(fileno(f), &infos) == -1) {
        perror("Erreur fstat");
        fclose(f);
        return 1;
    }
    fclose(f);

    off_t taille = infos.st_size;

    pthread_t thids[4];
    struct arg args[4];

    for (int i = 0; i < 4; ++i) {
        args[i].file = argv[1];
        args[i].begin = taille * i / 4;
        args[i].end   = taille * (i + 1) / 4;

        if (pthread_create(&thids[i], NULL, count_line, &args[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(thids[i], NULL);
    }

    printf("Nombre total de sauts de ligne dans %s : %d\n", argv[1], resultat);
    return 0;
}
