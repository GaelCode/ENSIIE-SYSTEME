#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int pid_pos, pid_neg;
volatile sig_atomic_t stop_pere = 0;

/* === Handlers de signaux === */

void handler_sigint_pere(int sig) {
    (void)sig;
    // On demande gentiment au père d'arrêter de lire
    stop_pere = 1;
}

void handler_sigusr1_fils(int sig) {
    (void)sig;
    // Message demandé pour les fils
    fprintf(stderr, "fils:%d: fin due à la réception de SIGUSR1\n", getpid());
    _exit(0);   // _exit pour éviter les soucis dans un handler
}

/* === Code des fils === */

void fils(char *nickname, int fdin) {
    int x;

    // Le fils ignore SIGINT (Ctrl-C)
    signal(SIGINT, SIG_IGN);
    // Le fils réagit à SIGUSR1
    signal(SIGUSR1, handler_sigusr1_fils);

    while (1) {
        ssize_t r = read(fdin, &x, sizeof(x));
        if (r == 0) {
            // EOF sur le pipe : père ne nous enverra plus rien
            // (on pourrait aussi sortir ici)
            break;
        } else if (r < 0) {
            perror("read");
            exit(1);
        } else if (r != sizeof(x)) {
            // lecture partielle : on continue pour compléter
            continue;
        }

        // comportement d'origine : on affiche l'entier lu
        printf("%s:%d: %d\n", nickname, getpid(), x);

        // si tu veux garder le comportement original, tu peux
        // aussi arrêter sur x == 0 :
        if (x == 0) break;
    }
    exit(0);
}

/* === Code du père === */

void pere(char *nickname, int pos[], int neg[]) {
    int x;

    // Le père écrit ses messages sur stderr
    dup2(2, 1);

    // Le père réagit à SIGINT (Ctrl-C)
    signal(SIGINT, handler_sigint_pere);

    while (!stop_pere) {
        printf("entrez un entier: ");
        fflush(stdout);

        int r = scanf("%d", &x);
        if (r == 1) {
            // Lecture OK : on route comme dans le forkPN original
            if (x >= 0) write(pos[1], &x, sizeof(x));
            if (x <= 0) write(neg[1], &x, sizeof(x));
            // (ici on ne s'arrête plus sur x == 0, c'est SIGINT qui arrête le père)
        } else if (r == EOF) {
            // Fin de stdin (par ex. redirection) : on sort aussi proprement
            fprintf(stderr, "%s:%d: EOF sur stdin\n", nickname, getpid());
            break;
        } else {
            // entrée invalide
            fprintf(stderr, "%s:%d: probleme lecture stdin.\n", nickname, getpid());
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
        }
    }

    // Le père arrête d'écrire dans les pipes
    close(pos[1]);
    close(neg[1]);

    // Il envoie SIGUSR1 aux deux fils
    kill(pid_pos, SIGUSR1);
    kill(pid_neg, SIGUSR1);

    // Il attend la mort de ses fils
    wait(NULL);
    wait(NULL);

    // Message final demandé
    fprintf(stderr, "pere:%d: terminaison\n", getpid());
    exit(0);
}

/* === main === */

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

    // fork du fils positifs
    if ((pid_pos = fork()) == 0) {
        // filsP : ne garde que pos[0]
        close(pos[1]);
        close(neg[0]);
        close(neg[1]);
        fils("filsP", pos[0]);
    } else if (pid_pos < 0) {
        perror("fork filsP");
        exit(3);
    }

    // fork du fils négatifs
    if ((pid_neg = fork()) == 0) {
        // filsN : ne garde que neg[0]
        close(neg[1]);
        close(pos[0]);
        close(pos[1]);
        fils("filsN", neg[0]);
    } else if (pid_neg < 0) {
        perror("fork filsN");
        exit(4);
    }

    // père : ne lit pas les pipes
    close(pos[0]);
    close(neg[0]);

    pere("pere", pos, neg);

    // On n'arrive normalement jamais ici
    fprintf(stderr, "Argh!!!\n");
    return 255;
}
