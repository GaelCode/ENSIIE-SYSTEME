#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

int pid_pos, pid_neg;

void fils(char *nickname, int fdin) {
    int x;
    while (1) {
        ssize_t r = read(fdin, &x, sizeof(x));

        if (r == 0) {
            // EOF sur le pipe -> plus de père qui écrit
            fprintf(stderr, "%s:%d: EOF, je me termine.\n", nickname, getpid());
            break;
        } else if (r < 0) {
            perror("read");
            exit(1);
        } else if (r != sizeof(x)) {
            // lecture partielle, on continue pour compléter
            continue;
        }

        // entier lu correctement
        printf("%s:%d: %d\n", nickname, getpid(), x);
    }
    exit(0);
}

void arret_inattendu(char *nickname,
                     pid_t fils_mort,
                     pid_t autre_fils,
                     int fd_pos,
                     int fd_neg) {
    // messages demandés
    fprintf(stderr, "%s:%d: fin inattendue du fils %d\n",
            nickname, getpid(), fils_mort);

    if (kill(autre_fils, SIGTERM) == -1) {
        // peut déjà être mort
        perror("kill");
    } else {
        fprintf(stderr, "%s:%d: arrêt du fils %d\n",
                nickname, getpid(), autre_fils);
    }

    fprintf(stderr, "%s:%d: terminaison\n", nickname, getpid());

    // on ferme les pipes (écriture)
    close(fd_pos);
    close(fd_neg);

    // on récupère les deux fils (quel que soit leur état)
    wait(NULL);
    wait(NULL);

    exit(0);
}

void pere(char *nickname,
          int fd_pos, int fd_neg,
          pid_t pid_pos_local, pid_t pid_neg_local) {
    int x;

    // le père écrit sur stderr
    dup2(2, 1);

    // ignorer SIGPIPE pour pouvoir tester write()
    signal(SIGPIPE, SIG_IGN);

    while (1) {
        printf("entrez un entier: ");
        fflush(stdout);

        int r = scanf("%d", &x);
        if (r == 1) {
            // entier lu
            if (x >= 0) {
                if (write(fd_pos, &x, sizeof(x)) != sizeof(x)) {
                    // écriture vers filsP a échoué -> filsP mort
                    perror("write pos");
                    arret_inattendu(nickname,
                                    pid_pos_local,  // fils mort
                                    pid_neg_local,  // autre fils à tuer
                                    fd_pos, fd_neg);
                }
            }
            if (x <= 0) {
                if (write(fd_neg, &x, sizeof(x)) != sizeof(x)) {
                    // écriture vers filsN a échoué -> filsN mort
                    perror("write neg");
                    arret_inattendu(nickname,pid_neg_local, pid_pos_local, fd_pos, fd_neg);
                }
            }
        } else if (r == EOF) {
            // fin de fichier sur stdin : cas "normal"
            fprintf(stderr, "%s:%d: EOF sur stdin, fin normale.\n",
                    nickname, getpid());
            break;
        } else {
            // entrée invalide
            fprintf(stderr, "%s:%d: probleme lecture stdin.\n",
                    nickname, getpid());
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
        }
    }

    close(fd_pos);
    close(fd_neg);

    wait(NULL);
    wait(NULL);

    exit(0);
}

int main(int argc, char *argv[]) {
    int pos[2];
    int neg[2];

    if (argc > 1) {
        fprintf(stderr, "usage: %s\n", argv[0]);
        exit(1);
    }
    if (pipe(pos) == -1 || pipe(neg) == -1) {
        perror("[pipe]");
        exit(2);
    }

    if ((pid_pos = fork()) == 0) {
        close(pos[1]);    
        close(neg[0]);    
        close(neg[1]);
        fils("filsP", pos[0]);
    } else if (pid_pos < 0) {
        perror("fork filsP");
        exit(3);
    }

    // filsN (negatifs et 0)
    if ((pid_neg = fork()) == 0) {
        // on est dans filsN
        close(neg[1]);    // ne lit que sur neg[0]
        close(pos[0]);    // inutiles
        close(pos[1]);
        fils("filsN", neg[0]);
    } else if (pid_neg < 0) {
        perror("fork filsN");
        exit(4);
    }

    // père
    close(pos[0]);  // ne lit pas les pipes
    close(neg[0]);

    pere("pere", pos[1], neg[1], pid_pos, pid_neg);

    // ne devrait jamais être atteint
    fprintf(stderr, "Argh!!!\n");
    return 255;
}