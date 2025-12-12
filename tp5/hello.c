#include <unistd.h>
#include <stdio.h>

int main(void) {

    int pid1 = fork();
    if(pid1 == -1) {
        perror("Erreur fork() fils 1");
        return 1;
    }
    

    int pid2 = fork();
    if(pid2 == -1) {
        perror("Erreur fork() fils 2");
        return 1;
    }

    if (pid1 == 0) { 
        sleep(2);
        printf("\n");
        return 0;
    }

    if (pid2 == 0) { 
        sleep(1);
        printf(" world");
        return 0;
    }

    printf("hello");
    fflush(stdout); // on force l'affichage du buffer à la place du \n
    wait(NULL);
    wait(NULL);
    return 0;
}