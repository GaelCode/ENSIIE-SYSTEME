#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define FIFO_TO_SERVER "fifo_to_server"
#define FIFO_TO_CLIENT "fifo_to_client"

// (6) Gestionnaire pour SIGPIPE
void handle_sigpipe(int sig) {
    printf("Signal attrapé : le serveur a fermé le pipe (SIGPIPE).\n");
    exit(1);
}

int main() {
    char message[256];
    char reponse[256];
    int fd_out, fd_in;

    // (6) Installation du gestionnaire de signal
    signal(SIGPIPE, handle_sigpipe);

    // (a) Ouverture des pipes
    fd_out = open(FIFO_TO_SERVER, O_WRONLY);
    if (fd_out < 0) {
        perror("Erreur ouverture serveur (est-il lancé ?)");
        exit(1);
    }
    fd_in = open(FIFO_TO_CLIENT, O_RDONLY);

    // (b) Saisie et envoi
    printf("Saisissez votre message pour le serveur : ");
    fgets(message, 256, stdin);
    write(fd_out, message, strlen(message) + 1);

    // (c) Lecture réponse
    read(fd_in, reponse, sizeof(reponse));
    printf("Réponse du serveur : %s\n", reponse);

    close(fd_in);
    close(fd_out);
    return 0;
}