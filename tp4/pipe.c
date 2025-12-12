#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


int main(void) {
    int fd[2];
    int nombre = 1;

    if (pipe(fd) == -1) {
        perror("erreur, création pipe");
        exit(1);
    }

    fcntl(fd[1], F_SETFL, O_NONBLOCK);

    while(1) {
        if (write(fd[1], "a", 1) == -1) {
            fprintf(stderr, "erreur dans l'écriture sur le pipe: %s\n", strerror(errno));
            exit(1);
        }
        printf("compteur = %d\n", nombre);
        nombre++;
    }

    close(fd[0]);
    close(fd[1]);
}

/* Question 2
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void) {
    int fd[2];

    if (pipe(fd) == -1) {
        perror("Erreur création pipe");
        exit(EXIT_FAILURE);
    }

    int taille = fcntl(fd[0], F_SETFL);
    if (taille == -1) {
        perror("Erreur fcntl");
        exit(EXIT_FAILURE);
    }

    printf("La taille d’un pipe non nommé est de %d octets.\n", taille);

    close(fd[0]);
    close(fd[1]);
    return 0;
}
*/

/* Question 3
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void) {
    const char *fifo_name = "monfifo";

    // Création du FIFO
    if (mkfifo(fifo_name, 0666) == -1) {
        perror("Erreur mkfifo");
        exit(EXIT_FAILURE);
    }

    // Ouverture en lecture et écriture
    int fd_read = open(fifo_name, O_RDONLY | O_NONBLOCK);
    if (fd_read == -1) {
        perror("Erreur open lecture");
        unlink(fifo_name);
        exit(EXIT_FAILURE);
    }

    int fd_write = open(fifo_name, O_WRONLY);
    if (fd_write == -1) {
        perror("Erreur open écriture");
        close(fd_read);
        unlink(fifo_name);
        exit(EXIT_FAILURE);
    }

    // Récupération de la taille du buffer du FIFO
    int taille = fcntl(fd_read, F_GETPIPE_SZ);
    if (taille == -1) {
        perror("Erreur fcntl");
        close(fd_read);
        close(fd_write);
        unlink(fifo_name);
        exit(EXIT_FAILURE);
    }

    printf("La taille d’un pipe nommé (FIFO) est de %d octets.\n", taille);

    // Nettoyage
    close(fd_read);
    close(fd_write);
    unlink(fifo_name);

    return 0;
}
*/