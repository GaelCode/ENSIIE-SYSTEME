# 8 — Processus

## Introduction

La section 8 présente trois grands thèmes :

1. **Les processus Unix** (création, fonctionnement, zombies, attentes…)
2. **Les threads POSIX** (API, création, synchronisation…)
3. **Les fonctions réentrantes et thread-safe** (sécurité en concurrence)

---

## 8.1 Processus Unix

Un **processus** est une instance d’un programme en cours d’exécution.
Deux types importants sont décrits :

* **Processus lourd** (créé par `fork`)
* **Processus léger** (créé par `sys_clone`)

---

### 8.1.1 Processus lourd

### **Syntaxe**

```c
pid_t fork(void);
```

Crée un **clone complet** du processus courant.
Retour :

* `0` dans le *fils*
* `pid du fils` dans le *père*
* `-1` en cas d’erreur (dans le père uniquement) 

### **Exemple d’usage**

```c
int pid = fork();
if (pid == 0) { // fils
    execlp("ls", "ls", "-l", NULL);
    exit(1);
}
// père
if (pid < 0) { exit(1); }
```

### **Principe de fonctionnement du fork**

Lors de la duplication : 

| Composant                      | Effet                               |
| ------------------------------ | ----------------------------------- |
| **PID**                        | nouveau dans le fils                |
| **Espace virtuel utilisateur** | cloné                               |
| **MMU**                        | pointe vers un clone                |
| **Mémoire partagée**           | conservée                           |
| **Flux**                       | partagés (mêmes curseurs)           |
| **Signaux**                    | reçus effacés, gestionnaires copiés |
| **Registres**                  | identiques sauf la valeur de retour |

Le père et le fils exécutent le *même code* mais dans *des espaces mémoire distincts*.

---

### 8.1.2 Processus léger (`sys_clone`)

Contrairement à `fork`, `sys_clone` permet de **partager certaines ressources** (mémoire, fichiers, signaux…).

### **Syntaxe**

```c
pid_t sys_clone(int flag, void* stack, ...);
```

Le flag décrit ce qui est **partagé** (TLS, espace mémoire, fichiers…).
Le fils démarre avec une **pile donnée**. 

C’est la base du fonctionnement des **threads POSIX**.

---

### 8.1.4 Processus zombie

Un processus devient **zombie** lorsque :

1. Il *se termine*,
2. mais son père **n’a pas récupéré** sa terminaison via `wait` ou `waitpid`.
   Le noyau libère tout sauf :

* le **PID**
* le **code de retour**

Si le père meurt avant le fils, le fils est adopté par `init`.
La sortie d’un fils est consommée par :

* un gestionnaire de **SIGCHLD**
* un appel à **wait / waitpid** 

---

### 8.1.5 Exemple complet

Programme “hel” / “lo” utilisant `fork` et `wait` :

```c
pid = fork();

if (pid == 0) {      // fils
    write(1, "hel", 3);
    exit(0);
}

if (pid == -1) { ... }

wait(&status);
write(1, "lo\n", 3);
```



---

## 8.2 Threads POSIX

Les **Pthreads** (1995) permettent de créer des processus légers partageant le même espace mémoire.
Fonctionnalités :

* création / destruction
* synchronisation (mutex, sémaphores)
* signaux
* TLS (Thread Local Storage)
* ordonnancement


---

### 8.2.1 Introduction

Un thread POSIX est un **processus léger**.
Il utilise `sys_clone` en interne.
Un espace TLS est créé pour :

* les données internes du thread
* les variables `__thread`
* les errno indépendants


⚠️ **Tous les threads peuvent lire/écrire dans les piles et TLS des autres**, car tout est dans le même espace virtuel.

---

### 8.2.2 API

### **Création**

```c
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                   void *(*func)(void *), void *arg);
```

### **Terminaison**

```c
void pthread_exit(void *ret);
```

Un `return x;` dans la fonction du thread ≡ `pthread_exit(x)`.

Exemple :

```c
void* print(void* s) { printf("%s", (char*)s); return NULL; }

pthread_t th;
pthread_create(&th, NULL, print, "hel");
sleep(1);
printf("lo\n");
```



---

### **Attente d’un thread (`pthread_join`)**

```c
int pthread_join(pthread_t th, void **status);
```

Attend la fin du thread et récupère le statut.


---

### **Exclusion mutuelle (`mutex`)**

```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_lock(&mutex);
pthread_mutex_unlock(&mutex);
```

Utilisé pour protéger des variables partagées.
Exemple long montrant la corruption sans mutex (résultat variable) et la correction avec mutex (résultat = 0).


---

### 8.2.3 Threads POSIX sous Linux

Résumé essentiel : 

| Fonction                | Comportement                      |
| ----------------------- | --------------------------------- |
| `sys_clone`             | chaque thread = processus léger   |
| `exit(s)`               | termine **tout** le groupe        |
| `getpid()`              | renvoie le PID du *thread maître* |
| `syscall(SYS_gettid)`   | renvoie le vrai PID du thread     |
| `ps -Af`                | montre seulement les maîtres      |
| `ps -LAf`               | montre tous les threads           |
| gestionnaire de signaux | partagé pour tous les threads     |
| signal vers un thread   | doit être envoyé à **gettid()**   |

---

## 8.3 Fonctions réentrantes et thread-safe

### **Problème général**

On veut garantir qu’un code reste correct lorsqu'il est :

* interrompu (réentrance)
* exécuté en parallèle (thread-safe)


---

### 8.3.2 Définitions

| Type de fonction | Définition                                                                                       |
| ---------------- | ------------------------------------------------------------------------------------------------ |
| **Réentrante**   | Peut être appelée alors qu’un premier appel n'est pas terminé (ex : interruption par un signal). |
| **Thread-safe**  | Peut être appelée par plusieurs threads en même temps sans corruption.                           |



### Exemples :

* **Appels système** : *toujours thread-safe*
* **Fonctions libc** : thread-safe sauf mention contraire (existe une version `_r` ou `_unlocked`)


---

### 8.3.4 Exemple : lecture libc

La version classique `fread` pose un verrou à chaque lecture → entraîne une surcoût significatif.
Remplacement par `fread_unlocked` → beaucoup plus rapide.