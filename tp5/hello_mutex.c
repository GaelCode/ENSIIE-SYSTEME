#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;

void* t1_func(void* arg) {
    printf("h");
    pthread_mutex_unlock(&m2);
    pthread_mutex_lock(&m1);
    printf("o");
     
    return NULL;
}

void* t2_func(void* arg) {
    pthread_mutex_lock(&m2);
    printf("ell");
    pthread_mutex_unlock(&m1);
    

    pthread_mutex_unlock(&m2);
    return NULL;
}

int main() {

    pthread_t thids[2];
    pthread_mutex_lock(&m1);
    pthread_mutex_lock(&m2);

    pthread_create(&thids[0], NULL, t1_func, NULL);
    pthread_create(&thids[1], NULL, t2_func, NULL);

    for(int i = 0; i < 2; i++) {
        pthread_join(thids[i], NULL);
    }

    printf("\n");

    return 0;
}
