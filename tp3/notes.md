# Notes du cours liés au TP3 sur les flux (en C)

Un processus possède :

* un segment text qui contient les instructions
* un segment données qui contient les données globales du processus
* un segment pile qui contient la pile d'exécution

---

## 1. Notion de flux

Un **flux** est un canal de communication entre un programme et une source/destination (fichier, terminal, périphérique, etc.).

Chaque processus a 3 flux standards :

| N° | Nom      | Direction | Usage             |
| -- | -------- | --------- | ----------------- |
| 0  | `stdin`  | entrée    | clavier, fichier  |
| 1  | `stdout` | sortie    | écran, fichier    |
| 2  | `stderr` | sortie    | messages d’erreur |

---

## 2. Fonctions bas niveau (système)

Fonctions : `open`, `read`, `write`, `close`, `lseek`
→ Manipulent des **descripteurs de fichiers** (int).

```c
int fd = open("f.txt", O_RDONLY);
char buf[100];
ssize_t n = read(fd, buf, sizeof(buf));
write(1, buf, n); // écrit sur stdout
close(fd);
```

### Exemple : lecture d’un fichier en bas niveau

Parfait 👍 voici ton extrait corrigé et complété, en précisant clairement qu’il faut **vérifier les valeurs de retour** des appels système (`open`, `read`, `lseek`, `close`) — car **elles renvoient -1 en cas d’erreur et jamais 0** (0 signifie autre chose, comme fin de fichier pour `read`).

---

```c
int main(int argc, char *argv[]) {
    int fd;
    char buffer[128];
    ssize_t nbytes;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s fichier\n", argv[0]);
        return 1;
    }

    // ouverture en lecture seule
    // flags possibles :
    //   O_RDONLY   : lecture seule
    //   O_WRONLY   : écriture seule
    //   O_RDWR     : lecture + écriture
    //   O_CREAT    : crée le fichier s’il n’existe pas (nécessite 'mode')
    //   O_TRUNC    : tronque le fichier (vide) s’il existe
    //   O_APPEND   : écrit à la fin du fichier
    //   O_EXCL     : échec si le fichier existe déjà (avec O_CREAT)
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Erreur ouverture: %s\n", strerror(errno));
        return 1;
    }

    // lecture du fichier
    ssize_t nbytes = read(f, buffer, taille - 1);
    ssize_t wbytes = write(fw, buffer, taille - 1);
    while ((nbytes = read(fd, buffer, sizeof(buffer)-1)) > 0) {
        buffer[nbytes] = '\0';
        printf("%s", buffer);
    }

    // /!\ Toujours vérifier le retour de read : -1 = erreur, 0 = fin du fichier
    if (nbytes == -1)
        fprintf(stderr, "Erreur lecture: %s\n", strerror(errno));

    // /!\ Vérifier aussi que close() ne renvoie pas -1
    if (close(fd) == -1)
        fprintf(stderr, "Erreur fermeture: %s\n", strerror(errno));

    return 0;
}
```

---

### Déplacement dans un fichier : `lseek`

```c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main() {
    int fd = open("data.bin", O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Erreur ouverture: %s\n", strerror(errno));
        return 1; // /!\ Attention : bien vérifier que le retour n'est pas égal à -1 (sinon 0)
    }

    // lseek(fd, offset, origine)
    // origines possibles :
    //   SEEK_SET : depuis le début du fichier
    //   SEEK_CUR : depuis la position actuelle
    //   SEEK_END : depuis la fin du fichier

    off_t taille = lseek(fd, 0, SEEK_END);
    if (taille == -1) {
        fprintf(stderr, "Erreur lseek: %s\n", strerror(errno));
        close(fd);
        return 1;
    }

    // retour au début du fichier
    if (lseek(fd, 0, SEEK_SET) == -1) {
        fprintf(stderr, "Erreur repositionnement: %s\n", strerror(errno));
        close(fd);
        return 1;
    }

    printf("Taille du fichier : %ld octets\n", taille);

    if (close(fd) == -1)
        fprintf(stderr, "Erreur fermeture: %s\n", strerror(errno));

    return 0;
}
```

---

## 3. Fonctions haut niveau (libc)

Fonctions : `fopen`, `fread`, `fwrite`, `fprintf`, `fclose`
→ Manipulent des **pointeurs de type `FILE*`**, avec **tampon automatique**.

```c
FILE *f = fopen("f.txt", "r");
char buf[100];
while (fgets(buf, sizeof buf, f))
    printf("%s", buf);
fclose(f);

while (!feof(f)){
    fgetc(f);
}
```

### Exemple : même lecture mais en haut niveau

```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *f;
    char buffer[128];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s fichier\n", argv[0]);
        return EXIT_FAILURE;
    }

    f = fopen(argv[1], "r"); // r | r+(read+write)| w | w+(read+write)| a | a+(read+ajout)
    if (!f) { // /!\ Attention : bien vérifier que le retour est bon (sinon 0)
        perror("fopen");
        return EXIT_FAILURE;
    }

    while (fgets(buffer, sizeof(buffer), f))
        fputs(buffer, stdout);

    fclose(f);
    return EXIT_SUCCESS;
}
```

> 🔹 Plus simples, portables, adaptées aux fichiers texte.

### Déplacement dans un fichier : `fseek` / `ftell`

```c
FILE *f = fopen("data.txt", "r");
fseek(f, 0, SEEK_END);     // déplacement à la fin
long taille = ftell(f);    // donne la position actuelle
rewind(f);                 // équivalent à fseek(f, 0, SEEK_SET)
printf("Taille : %ld octets\n", taille);
fclose(f);
```

---

## 4. Redirection et duplication

Lorsqu’un programme démarre, les trois flux standards (`stdin`, `stdout`, `stderr`) sont déjà ouverts et associés au terminal. Il est possible de rediriger ces flux vers des fichiers ou d’autres flux à l’aide de fonctions de la libc ou d’appels système. 

### Rediriger la sortie standard vers un fichier

```c
freopen("out.txt", "w", stdout);
printf("Écrit dans out.txt\n");
fclose(stdout);
```

Cette redirection remplace le flux `stdout` par un flux associé au fichier `out.txt`. Toute écriture via `printf` sera dirigée vers ce fichier. On peut de la même façon rediriger `stdin` ou `stderr`.

```c
freopen("in.txt", "r", stdin);
freopen("err.txt", "w", stderr);
```

### Dupliquer un flux (bas niveau)

```c
int fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
dup2(fd, 1); // stdout → fichier
printf("redirection OK\n");
close(fd);
```

Le descripteur 1 (stdout) pointe désormais vers le fichier `out.txt`. Le contenu de `printf` sera écrit dans ce fichier.
Les redirections peuvent être combinées :

```c
int fd = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0644);
dup2(fd, 1);   // stdout → log.txt
dup2(fd, 2);   // stderr → log.txt
```

L’utilisation de `dup2` permet de dupliquer un descripteur vers un autre déjà existant. Ce mécanisme est à la base du chaînage des commandes dans un shell (ex. `cmd1 | cmd2`), où la sortie standard d’un processus devient l’entrée standard d’un autre par l’intermédiaire d’un **pipe**.


---

## 5. Différences résumées

| Aspect      | Bas niveau (`open`) | Haut niveau (`fopen`) |
| ----------- | ------------------- | --------------------- |
| Type        | int (fd)            | `FILE*`               |
| Tampon      | Non                 | Oui                   |
| Portabilité | Unix                | C standard            |
| Usage       | binaire, bas niveau | texte, simple         |

---

## 6. Exemple de copie minimaliste

### Version libc

```c
FILE *src=fopen("a.txt","r"), *dst=fopen("b.txt","w");
char c;
while ((c=fgetc(src))!=EOF) fputc(c,dst);
fclose(src); fclose(dst);
```

### Version système

```c
int s=open("a.txt",O_RDONLY), d=open("b.txt",O_WRONLY|O_CREAT,0644);
char c; while (read(s,&c,1)==1) write(d,&c,1);
close(s); close(d);
```