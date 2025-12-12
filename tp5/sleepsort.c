#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc==0) {
        perror("pas le bon nombre d'arguments");
        return 1;
    }

    for (int i = 0; i < argc; i++) {
        int pid = fork();
        if(pid == -1) {
            perror("erreur création pid"); 
            return 1;
        }
        if (pid == 0) {
            sleep(atoi(argv[i+1]));
            printf("%s ", argv[i+1]);
            return 0;
        }

        
    }
    for(int y = 0; y < argc; y++) {
        wait(NULL);
    }
    
    printf("\n");
    fflush(stdout);
    return 0;
}