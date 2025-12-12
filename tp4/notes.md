# Communication inter-processus (IPC)

## Signaux

### Définition et traitements possibles

Un **signal** est un **événement asynchrone** qu’un processus peut recevoir pour :

1. Être **interrompu** ou **terminé**
2. Exécuter une **action spécifique** (gestionnaire)
3. Être **suspendu** temporairement
4. **Ignorer** complètement l’événement

### Implémentation interne

Chaque processus possède :

* Une **table des signaux** associant à chaque signal un traitement.
* Lorsqu’un signal est émis :

  * Il est **marqué comme reçu**.
  * Le processus est **réveillé** s’il était suspendu.

### Principaux signaux (extraits)

| Nom                  | Numéro | Action par défaut | Description                        |
| -------------------- | ------ | ----------------- | ---------------------------------- |
| `SIGHUP`             | 1      | Term              | Déconnexion du terminal            |
| `SIGINT`             | 2      | Term              | Interruption clavier (Ctrl+C)      |
| `SIGQUIT`            | 3      | Core              | Quit depuis clavier                |
| `SIGKILL`            | 9      | Term              | Fin immédiate, non interceptable   |
| `SIGSEGV`            | 11     | Core              | Erreur d’accès mémoire             |
| `SIGPIPE`            | 13     | Term              | Écriture dans un pipe sans lecteur |
| `SIGTERM`            | 15     | Term              | Demande de terminaison             |
| `SIGUSR1`, `SIGUSR2` | 10, 12 | Term              | Signaux utilisateur                |
| `SIGCHLD`            | 17     | Ign               | Fils terminé ou arrêté             |

> 🚫 Les signaux `SIGKILL` et `SIGSTOP` **ne peuvent pas être interceptés ni ignorés**.

### Envoi et gestion

* **Envoyer un signal** :

  ```c
  int kill(pid_t pid, int sig);
  kill(getpid(), SIGKILL); // tue le processus lui-même
  ```
* **Fixer un gestionnaire** :

  ```c
  typedef void (*sighandler_t)(int);
  sighandler_t signal(int sig, sighandler_t handler);

  signal(SIGINT, SIG_IGN); // ignore Ctrl+C
  ```
* **Suspendre jusqu’à un signal** :

  ```c
  pause();
  ```
* **Déclencher un signal après un délai** :

  ```c
  alarm(5); // SIGALRM dans 5 secondes
  ```

---

## FIFO (First In First Out)

### Définition

Une **FIFO** (ou *pipe*) est un **canal de communication** entre processus :

* Taille maximale fixe
* Deux états : **vide** ou **plein**
* Deux rôles :

  * **Producteur / Écrivain**
  * **Consommateur / Lecteur**

> Un lecteur est bloqué si la FIFO est vide, et un écrivain est bloqué si elle est pleine.

### Types de FIFO

| Type                   | Communication | Machine          | Mode     |
| ---------------------- | ------------- | ---------------- | -------- |
| `tty`                  | N ↔ N         | locale           | octets   |
| `pipe`                 | N ↔ N         | locale (parenté) | octets   |
| `mkfifo`               | N ↔ N         | locale           | octets   |
| `message IPC`          | N ↔ N         | locale           | messages |
| `unix socket stream`   | 1 ↔ 1         | locale           | octets   |
| `unix socket datagram` | N → 1         | locale           | messages |
| `socket TCP`           | 1 ↔ 1         | distante         | octets   |
| `socket UDP`           | N → 1         | distante         | messages |

### Création et utilisation

* **Pipe anonyme** :

  ```c
  int fd[2];
  pipe(fd);
  // fd[0] : lecture, fd[1] : écriture
  ```
* **Pipe nommé (FIFO)** :

  ```bash
  mkfifo chemin
  # ou
  mknod chemin p
  ```

Puis on lit/écrit simplement avec `open()`, `read()`, `write()`.

### Particularités

* **Ouverture** :

  * Lecture seule bloquante s’il n’y a pas d’écrivains.
  * Écriture seule bloquante s’il n’y a pas de lecteurs.
* **Lecture** :

  * `read()` retourne `0` si plus d’écrivains.
* **Écriture** :

  * Sans lecteur → signal `SIGPIPE`.

---

## Mémoire partagée (SHM)

### Principe

En configurant les **MMU** (Memory Management Units), deux processus peuvent **partager une même zone mémoire physique**.
Cette zone est appelée **segment de mémoire partagée**.

> ⚙️ Les adresses virtuelles diffèrent, mais pointent vers la **même mémoire physique**.

### Outils SHM

| Outil            | Description                                         |
| ---------------- | --------------------------------------------------- |
| **Threads**      | Segment de données déjà partagé                     |
| **`mmap`**       | Crée un segment partagé pour des processus parentés |
| **System V IPC** | Voir `man svipc`                                    |
| **POSIX SHM**    | Voir `man shm_overview`                             |

---

## Sémaphores (SEM)

### Problème d’accès concurrent

Trois processus modifient la même variable `*p` :

```c
Processus1: *p += 1;
Processus2: *p += 3;
Processus3: *p += 5;
```

Selon l’ordre d’exécution, le résultat final peut varier (non déterministe).

### Définition

Un **sémaphore** est une structure composée de :

* Un **état** : `LIBRE` ou `BLOQUÉ`
* Une **file de processus en attente**
* Deux opérations :

  * `P()` (wait) : bloque si occupé
  * `V()` (signal) : libère ou réveille un processus

### Exemple : exclusion mutuelle

```c
P(mutex); // sert à bloquer la mutex, si utilisé avant alors il attend
*p += 1;
V(mutex); // sert à débloquer la mutex
```

Assure qu’un seul processus accède à la section critique à la fois.

### Synchronisation / Rendez-vous

Les sémaphores peuvent aussi **synchroniser** plusieurs processus :

```c
S1 ← {BLOQUÉ, vide}
S2 ← {BLOQUÉ, vide}

P1 : V(S2); P(S1); RDV
P2 : P(S2); V(S1); RDV
```