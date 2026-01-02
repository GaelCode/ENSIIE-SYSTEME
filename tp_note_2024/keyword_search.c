#define _GNU_SOURCE // Pour strcasestr
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

// Variables globales pour être accessibles par le gestionnaire de signaux
pid_t pid_f1 = -1, pid_f2 = -1;
int f1_interrupted = 0, f2_interrupted = 0;

// Gestionnaire du signal SIGCHLD
void handle_sigchld(int sig) {
    int status;
    pid_t pid;

    // Récupère l'état de n'importe quel fils qui s'est terminé
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFSIGNALED(status)) { // Si le fils a été tué par un signal
            if (pid == pid_f1) {
                f1_interrupted = 1;
                printf("\nFils 1 (PID %d) s'est terminé (signal SIGTERM).\n", pid);
                if (pid_f2 > 0) {
                    printf("Fils 2 (PID %d) a été arrêté par le père (signal SIGTERM).\n", pid_f2);
                    kill(pid_f2, SIGTERM);
                }
            } else if (pid == pid_f2) {
                f2_interrupted = 1;
                printf("\nFils 2 (PID %d) s'est terminé (signal SIGTERM).\n", pid);
                if (pid_f1 > 0) {
                    printf("Fils 1 (PID %d) a été arrêté par le père (signal SIGTERM).\n", pid_f1);
                    kill(pid_f1, SIGTERM);
                }
            }
        }
    }
}

// Fonction exécutée par les fils
void run_child(int read_pipe, int write_pipe, int id, char *keywords[]) {
    char buffer[1024];
    int count = 0;
    FILE *stream = fdopen(read_pipe, "r");

    // Lecture ligne par ligne depuis le pipe du père
    while (fgets(buffer, sizeof(buffer), stream)) {
        for (int i = 0; keywords[i] != NULL; i++) {
            if (strcasestr(buffer, keywords[i])) {
                count++;
                break; // On compte la ligne une seule fois
            }
        }
    }

    // Envoi du résultat au format "ID:count"
    dprintf(write_pipe, "%d:%d\n", id, count);
    
    fclose(stream);
    close(write_pipe);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <fichier>\n", argv[0]);
        return 1;
    }

    int pipe_f1[2], pipe_f2[2], result_pipe[2];
    
    // Création des pipes
    pipe(pipe_f1);
    pipe(pipe_f2);
    pipe(result_pipe);

    // Configuration de SIGCHLD
    struct sigaction sa;
    sa.sa_handler = handle_sigchld;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);

    // Ignorer SIGPIPE pour éviter que le père ne meurt si un fils est tué pendant l'écriture
    signal(SIGPIPE, SIG_IGN);

    // Création du Fils 1 (Erreurs)
    pid_f1 = fork();
    if (pid_f1 == 0) {
        close(pipe_f1[1]); close(pipe_f2[0]); close(pipe_f2[1]); close(result_pipe[0]);
        char *keys[] = {"error", "warning", NULL};
        run_child(pipe_f1[0], result_pipe[1], 1, keys);
    }

    // Création du Fils 2 (Succès)
    pid_f2 = fork();
    if (pid_f2 == 0) {
        close(pipe_f2[1]); close(pipe_f1[0]); close(pipe_f1[1]); close(result_pipe[0]);
        char *keys[] = {"success", "completed", NULL};
        run_child(pipe_f2[0], result_pipe[1], 2, keys);
    }

    // Processus Père
    printf("Fils 1 lancé avec PID %d pour les erreurs.\n", pid_f1);
    printf("Fils 2 lancé avec PID %d pour les succès.\n", pid_f2);

    close(pipe_f1[0]); close(pipe_f2[0]); close(result_pipe[1]);

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Erreur ouverture fichier");
        kill(pid_f1, SIGTERM); kill(pid_f2, SIGTERM);
        return 1;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        // Envoi de la ligne aux deux fils
        write(pipe_f1[1], line, strlen(line));
        write(pipe_f2[1], line, strlen(line));
    }

    fclose(file);
    close(pipe_f1[1]);
    close(pipe_f2[1]);

    // Attente de la fin des fils
    while (waitpid(pid_f1, NULL, WNOHANG) == 0 || waitpid(pid_f2, NULL, WNOHANG) == 0) {
        usleep(100000); // Petite pause
    }

    // Lecture des résultats dans le result_pipe
    int res1 = 0, res2 = 0;
    char res_buf[64];
    FILE *res_stream = fdopen(result_pipe[0], "r");
    while (fgets(res_buf, sizeof(res_buf), res_stream)) {
        int id, val;
        if (sscanf(res_buf, "%d:%d", &id, &val) == 2) {
            if (id == 1) res1 = val;
            if (id == 2) res2 = val;
        }
    }

    // Si un fils a été tué, son résultat est forcé à 0
    if (f1_interrupted) res1 = 0;
    if (f2_interrupted) res2 = 0;

    printf("\n--- Statistiques Finales ---\n");
    printf("Nombre de lignes contenant des mots-clés d'erreur : %d %s\n", 
            res1, f1_interrupted ? "(fils interrompu)" : "");
    printf("Nombre de lignes contenant des mots-clés de succès : %d %s\n", 
            res2, f2_interrupted ? "(fils interrompu)" : "");

    close(result_pipe[0]);
    return 0;
}