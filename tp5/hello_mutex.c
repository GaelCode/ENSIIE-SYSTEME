#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

struct arg {
    int begin;
    int end;
};

char* texte = "hello\n";

// Mutex utilisés comme sémaphores pour ordonner l’affichage
pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m3 = PTHREAD_MUTEX_INITIALIZER;

void* afficher(void* a) {
    struct arg* args = (struct arg*) a;

    // ordre : h -> ell -> o

    // 1) attendre autorisation pour la portion [begin, end[
    if (args->begin == 0) {  
        // thread 1 : "h"
        pthread_mutex_lock(&m1);
        write(1, texte + args->begin, args->end - args->begin);
        pthread_mutex_unlock(&m2); // autorise "ell"
    }
    else if (args->begin == 1) {  
        // thread 2 : "ell"
        pthread_mutex_lock(&m2);
        write(1, texte + args->begin, args->end - args->begin);
        pthread_mutex_unlock(&m3); // autorise "o"
    }
    else if (args->begin == 4) {  
        // thread 1 : "o"
        pthread_mutex_lock(&m3);
        write(1, texte + args->begin, args->end - args->begin);
    }

    return NULL;
}

int main() {

    // On verrouille m2 et m3 pour bloquer ell et o
    pthread_mutex_lock(&m2);
    pthread_mutex_lock(&m3);

    pthread_t th1, th2;
    struct arg a1 = {0, 1};   // "h"
    struct arg a2 = {1, 4};   // "ell"
    struct arg a3 = {4, 5};   // "o"

    // thread pour 'h'
    pthread_create(&th1, NULL, afficher, &a1);

    // thread pour 'ell'
    pthread_create(&th2, NULL, afficher, &a2);

    // le dernier morceau "o" doit être fait par le même thread que "h"
    pthread_t th3;
    pthread_create(&th3, NULL, afficher, &a3);

    // autoriser "h"
    pthread_mutex_unlock(&m1);

    pthread_join(th1, NULL);
    pthread_join(th2, NULL);
    pthread_join(th3, NULL);

    // newline final par le parent
    write(1, texte + 5, 1);

    return 0;
}
