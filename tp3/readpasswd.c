#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

int readpassword(char* pass) {
    char buffer[16];  // 15 caractères + '\0'
    int n = 0;

    // Récupérer le nom de la tty associée à l'entrée standard
    char* ttypath = ttyname(0);
    if (ttypath == NULL) { 
        fprintf(stderr, "readpassword(): erreur récupération du terminal : %s\n", strerror(errno)); 
        return -1; 
    }

    // Ouvrir la tty en lecture/écriture
    int fdStdin = open(ttypath, O_RDWR);
    if (fdStdin == -1) {
        fprintf(stderr, "readpassword(): erreur ouverture tty : %s\n", strerror(errno)); 
        return -1; 
    }

    // Mettre le terminal en mode raw et sans écho
    system("/bin/stty raw -echo");

    // Afficher le prompt
    write(fdStdin, "password: ", 10);

    // Lecture caractère par caractère jusqu’à '\n' ou '\r'
    for (int i = 0; i < 15; i++) {
        char c;
        ssize_t r = read(fdStdin, &c, 1);

        if (r == -1) {
            fprintf(stderr, "readpassword(): erreur lecture : %s\n", strerror(errno));
            system("/bin/stty -raw echo");
            close(fdStdin);
            return -1;
        }

        if (c == '\n' || c == '\r') {
            n = i;
            break;
        }

        buffer[i] = c;
        n = i + 1;

        // Afficher un * pour chaque caractère lu
        write(fdStdin, "*", 1);
    }

    // Fin de ligne après saisie
    write(fdStdin, "\r\n", 2);

    // Remettre le mode normal
    system("/bin/stty -raw echo");

    // Stocker dans pass
    for (int i = 0; i < n && i < 15; i++) {
        pass[i] = buffer[i];
    }

    // Ajouter le caractère de fin '\0'
    if (n >= 15)
        pass[15] = '\0';
    else
        pass[n] = '\0';

    close(fdStdin);
    return n;
}

int main(void) {
    char *pass = malloc(16 * sizeof(char));
    if (!pass) {
        fprintf(stderr, "Erreur d’allocation mémoire\n");
        return 1;
    }

    int n = readpassword(pass);
    if (n >= 0)
        printf("Mot de passe lu (%d caractères) : \"%s\"\n", n, pass);
    else
        fprintf(stderr, "Erreur de lecture du mot de passe.\n");

    free(pass);
    return 0;
}