#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int pid_pos, pid_neg;

void fils(char* nickname, int fdin) {
    int x;
    while (1) {
        ssize_t r = read(fdin, &x, sizeof(x));

        if (r == 0) {
            // fin de fichier sur le pipe : plus d’écrivains -> on s’arrête
            fprintf(stderr, "%s:%d: EOF, je me termine.\n", nickname, getpid());
            break;
        } else if (r < 0) {
            perror("read");
            exit(1);
        } else if (r != sizeof(x)) {
            // lecture partielle : on peut continuer, le prochain read complètera
            continue;
        }

        // ici on a bien lu un entier complet
        printf("%s:%d: %d\n", nickname, getpid(), x);
        // dans cette variante, on ne s’arrête pas sur une valeur particulière,
        // seulement sur EOF.
    }
    exit(0);
}

void pere(char* nickname, int fd_pos, int fd_neg) {
    int x;

    // redirige stdout vers stderr (comme dans la version originale)
    dup2(2, 1);

    while (1) {
        printf("entrez un entier: ");
        fflush(stdout);

        int r = scanf("%d", &x);
        if (r == 1) {
            if (x >= 0) {
                if (write(fd_pos, &x, sizeof(x)) != sizeof(x)) {
                    perror("write pos");
                }
            }
            if (x <= 0) {
                if (write(fd_neg, &x, sizeof(x)) != sizeof(x)) {
                    perror("write neg");
                }
            }
        } else if (r == EOF) { // soit CTRL+D
            // fin de fichier sur stdin : le père s’arrête
            fprintf(stderr, "%s:%d: EOF sur stdin, je m’arrête.\n", nickname, getpid());
            break;
        } else {
            // problème de lecture (ex : texte au lieu de nombre)
            fprintf(stderr, "%s:%d: probleme lecture stdin.\n", nickname, getpid());
            int c;
            // vider la ligne en cours
            while ((c = getchar()) != '\n' && c != EOF);
            // et continuer
        }
    }

    // très important : fermer les extrémités d’écriture des pipes
    // pour que les fils voient EOF
    close(fd_pos);
    close(fd_neg);

    // attendre la fin des deux fils
    wait(NULL);
    wait(NULL);

    exit(0);
}

int main(int argc, char* argv[]) {
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

    // premier fork : filsP (positifs)
    if ((pid_pos = fork()) == 0) {
        // Processus filsP

        // Il ne va que LIRE sur pos[0]
        // donc on ferme tout ce qu'il n'utilise pas :
        close(pos[1]);    // écriture du pipe pos
        close(neg[0]);    // inutilisé
        close(neg[1]);    // inutilisé

        fils("filsP", pos[0]);
    }
    else if (pid_pos < 0) {
        perror("fork filsP");
        exit(3);
    }

    // deuxième fork : filsN (négatifs)
    if ((pid_neg = fork()) == 0) {
        // Processus filsN

        // Il ne va que LIRE sur neg[0]
        close(neg[1]);    // écriture du pipe neg
        close(pos[0]);    // inutilisé
        close(pos[1]);    // inutilisé

        fils("filsN", neg[0]);
    }
    else if (pid_neg < 0) {
        perror("fork filsN");
        exit(4);
    }

    // Processus père
    // Il ne lit pas les pipes, donc on ferme les extrémités de lecture :
    close(pos[0]);
    close(neg[0]);

    pere("pere", pos[1], neg[1]);

    // En principe on ne revient jamais ici
    fprintf(stderr, "Argh!!!\n");
    return 255;
}
