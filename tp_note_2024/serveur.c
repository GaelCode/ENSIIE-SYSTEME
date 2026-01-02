#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define FIFO_TO_SERVER "fifo_to_server"
#define FIFO_TO_CLIENT "fifo_to_client"

// Gestionnaire pour supprimer les pipes à la fermeture (CTRL+C)
void cleanup(int sig) {
    printf("\nNettoyage des pipes et fermeture du serveur...\n");
    unlink(FIFO_TO_SERVER);
    unlink(FIFO_TO_CLIENT);
    exit(0);
}

int main() {
    char buffer[256];
    int fd_in, fd_out;

    // (a) Création des pipes nommés
    mkfifo(FIFO_TO_SERVER, 0666);
    mkfifo(FIFO_TO_CLIENT, 0666);
    
    signal(SIGINT, cleanup); // (e) Gestion de SIGINT

    printf("Serveur en attente...\n");

    // (b) Ouverture en lecture bloquante
    fd_in = open(FIFO_TO_SERVER, O_RDONLY);
    // Ouverture pour répondre
    fd_out = open(FIFO_TO_CLIENT, O_WRONLY);

    // (c) Lecture du message
    read(fd_in, buffer, sizeof(buffer));
    printf("Message reçu du client : %s\n", buffer);

    // (d) Réponse au client
    char *reponse = "Message bien reçu par le serveur !";
    write(fd_out, reponse, 35);

    close(fd_in);
    close(fd_out);
    cleanup(0);
    return 0;
}