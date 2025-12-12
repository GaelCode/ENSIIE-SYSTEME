#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <assert.h>

void sigpipe(int sig) {
    assert(sig == SIGPIPE);
    fprintf(stderr, "SIGPIPE received... aborting in 5 seconds\n");
    sleep(5);
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 1) {
        fprintf(stderr, "usaage: %s\n", argv[0]);
        exit(1);
    }

    fprintf(stderr, "%s: started\n", argv[0]);
    signal(SIGPIPE, sigpipe);
    
    while (1) {
        char c;
        int statut = read(0, &c, 1);
        if (statut == -1) {
            perror("[read]");
            exit(2);
        } else if (statut == 0) {
            write(2, ".", 1);
            usleep(100000); /* 0.1s */
        } else { /* statut == 1 */
            write(1, &c, 1);
        }
    }

    return 0;
}
