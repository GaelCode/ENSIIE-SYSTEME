#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* t1_func(void* arg) {
    write(1, "h", 1);
    sleep(1); 
    
    write(1, "o", 1);
    return NULL;
}

void* t2_func(void* arg) {
    sleep(0.5);
    write(1, "ell", 3);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, t1_func, NULL);
    pthread_create(&t2, NULL, t2_func, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    write(1, "\n", 1);
    return 0;
}
