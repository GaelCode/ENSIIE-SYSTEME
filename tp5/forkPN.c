#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int pid_pos, pid_neg;

void fils(char* nickname, int fdin) {
    int x;
    while (1) {
        if (read(fdin, &x, sizeof(x)) != sizeof(x)) continue;
        printf("%s:%d: %d\n", nickname, getpid(), x);
        if (x == 0) break;
    }
    exit(0);
}

void pere(char* nickname, int pos[], int neg[]) {
    int x;
    dup2(2, 1);
    while (1) {
        printf("entrez un entier: ");
        if (scanf("%d", &x) != 1) {
            fprintf(stderr, "%s:%d: probleme lecture stdin.\n", nickname, getpid());
            while (getchar() != '\n');
            continue;
        }
        if (x >= 0) write(pos[1], &x, sizeof(x));
        if (x <= 0) write(neg[1], &x, sizeof(x));
        if (x == 0) break;
    }
    wait(NULL);
    wait(NULL);
    exit(0);
}

int main(int argc, char* argv[]) {
    int pos[2];
    int neg[2];

    if (argc > 1) { fprintf(stderr, "usage: %s\n", argv[0]); exit(1); }
    if (pipe(pos) == -1 || pipe(neg) == -1) { perror("[pipe]"); exit(2); }

    if ((pid_pos = fork()) == 0)      fils("filsP", pos[0]);
    else if ((pid_neg = fork()) == 0) fils("filsN", neg[0]);
    else                              pere("pere", pos, neg);

    fprintf(stderr, "Argh!!!\n");
    return 255;
}
