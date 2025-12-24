# TPs ISE - Système d'Exploitation et Shell

Ce dossier contient tous les travaux pratiques pour le cours ISE (Informatique, Système et Exploitation).

## Structure du projet

```
MEIM/
├── ISE_TP1/          # Les bases du Shell
│   ├── README.md
│   ├── exercice3_creation_fichier_a.sh
│   ├── exercice6_calculs.sh
│   └── exercice8_manipulation_texte.sh
│
├── ISE_TP2/          # Scripts Shell avancés
│   ├── README.md
│   ├── exercice1/    # Punitions
│   ├── exercice2/    # Wrapper
│   ├── exercice3/    # Tri d'arguments
│   └── exercice4-5/  # s3tool et applications
│
├── ISE_TP3/          # Flux (Streams)
│   ├── README.md
│   ├── exercice1/    # Flux noyau (open, read, write, lseek, fstat)
│   ├── exercice2/    # Flux libc (fopen, fread, fwrite, fseek)
│   ├── exercice3/    # Redirections (execl, dup, dup2)
│   ├── exercice4/    # Wrapper C (execv, setenv)
│   └── exercice5/    # Mot de passe sécurisé (ttyname, stty)
│
├── ISE_TP4/          # Communication inter-processus
│   ├── README.md
│   ├── Makefile
│   ├── exercice1/    # Signaux
│   ├── exercice2/    # FIFO et fichiers réguliers
│   ├── exercice3/    # Pipes
│   └── exercice4/    # pause et alarm
│
└── GUIDE_ISE.md      # Ce fichier
```

## Vue d'ensemble des TPs

### TP1 - Les bases du Shell
**Objectif** : Maîtriser les commandes de base du shell et la manipulation de fichiers

**Compétences** :
- Navigation dans le système de fichiers (cd, pwd)
- Manipulation de fichiers (cp, mv, rm, mkdir)
- Redirections et pipes (>, >>, <, |)
- Variables d'environnement ($HOME, $PATH)
- Gestion des processus (fg, bg, jobs)
- Traitement de texte (grep, sed, cut, head, tail)

**Commandes importantes** :
```bash
cd ~/sys/tp1              # Navigation
mkdir -pv chemin          # Création de répertoires
cp -rv source dest        # Copie récursive
echo -n 'texte' > file    # Redirection
cat file | grep motif     # Pipe
export PATH="$PATH:dir"   # Modification de PATH
```

### TP2 - Scripts Shell
**Objectif** : Écrire des scripts shell robustes avec boucles, fonctions et tests

**Compétences** :
- Boucles (while, for, until)
- Conditions (if, test, [ ])
- Fonctions shell
- Gestion des arguments (\$#, $@, shift, set)
- Variables locales et d'environnement
- Source de fichiers (. et source)

**Concepts clés** :
```bash
# Boucles
while [ condition ] ; do commandes ; done

# Conditions
if [ -f file ] ; then commandes ; fi

# Fonctions
ma_fonction() {
    local var="valeur"
    return 0
}

# Arguments
[ $# -eq 3 ]      # Vérifier le nombre d'arguments
shift             # Décaler les arguments
set -- $nouvelle_liste  # Réinitialiser les arguments
```

### TP3 - Flux (Streams)
**Objectif** : Maîtriser les flux noyau et libc, les redirections et exec

**Compétences** :
- Flux noyau : open(), read(), write(), lseek(), fstat(), close()
- Flux libc : fopen(), fread(), fwrite(), fseek(), ftell(), fclose()
- Descripteurs de fichiers (fd) et FILE*
- Redirections : dup(), dup2()
- Exec : execl(), execv() et variantes
- Modification d'environnement : setenv(), getenv()
- Contrôle du terminal : ttyname(), stty

**Fonctions clés** :
```c
// Flux noyau
int fd = open("file", O_RDONLY);
read(fd, buf, n);
write(fd, buf, n);
lseek(fd, 0, SEEK_END);
close(fd);

// Flux libc
FILE *f = fopen("file", "r");
fread(buf, 1, n, f);
fwrite(buf, 1, n, f);
fseek(f, 0, SEEK_END);
long size = ftell(f);
fclose(f);

// Redirections
int saved = dup(1);        // Sauvegarder stdout
dup2(fd, 1);               // Rediriger stdout vers fd
dup2(saved, 1);            // Restaurer

// Exec
execl("/bin/ls", "ls", "-l", NULL);
execv("/bin/prog", argv);
```

**Différences importantes** :
- **Flux noyau** : Pas de buffering, contrôle total, appels système directs
- **Flux libc** : Buffering automatique, plus pratique, couche au-dessus du noyau
- **dup2()** : Permet de rediriger n'importe quel descripteur (stdin, stdout, stderr...)
- **exec()** : Remplace le processus courant, ne retourne jamais sauf erreur

### TP4 - Communication inter-processus
**Objectif** : Comprendre les mécanismes de communication entre processus Unix

**Compétences** :
- Signaux (signal, kill, pause, alarm)
- Pipes et FIFO (pipe, mkfifo, read, write)
- Contrôle de fichiers (fcntl)
- Gestion des E/S bloquantes et non-bloquantes

**Fonctions système** :
```c
// Signaux
signal(SIGINT, handler);
kill(pid, SIGUSR1);
pause();
alarm(10);

// Pipes
pipe(pipefd);
read(fd, buf, count);
write(fd, buf, count);

// Contrôle
fcntl(fd, F_GETPIPE_SZ);
```

## Installation et configuration

### Prérequis
- Shell : bash ou zsh
- Compilateur : gcc avec support C99
- Outils : make, xterm (optionnel)

### Configuration initiale

```bash
# Se placer dans le dossier MEIM
cd ~/MEIM

# Rendre les scripts exécutables
chmod +x ISE_TP1/*.sh
chmod +x ISE_TP2/exercice*/*
chmod +x ISE_TP2/exercice*/.*

# Compiler tous les programmes C du TP4
cd ISE_TP4
make
cd ..
```

### Ajouter les scripts au PATH (optionnel)

```bash
# Ajouter temporairement (valable pour la session courante)
export PATH="$HOME/MEIM/ISE_TP2/exercice1:$PATH"
export PATH="$HOME/MEIM/ISE_TP2/exercice2:$PATH"
export PATH="$HOME/MEIM/ISE_TP2/exercice3:$PATH"
export PATH="$HOME/MEIM/ISE_TP2/exercice4-5:$PATH"

# Ou ajouter définitivement dans ~/.bashrc
echo 'export PATH="$HOME/MEIM/ISE_TP2/exercice1:$PATH"' >> ~/.bashrc
```

## Utilisation

### TP1 - Tests des exercices

```bash
cd ISE_TP1

# Exercice 3 : Créer un fichier avec 1000 'A'
./exercice3_creation_fichier_a.sh

# Exercice 6 : Calculs arithmétiques
./exercice6_calculs.sh

# Exercice 8 : Manipulation de texte
./exercice8_manipulation_texte.sh
```

### TP2 - Tests des scripts

```bash
cd ISE_TP2

# Exercice 1 : Punitions
./exercice1/punition.sh
./exercice1/punition.sh "Hello"
./exercice1/punition.sh 3 5 "*"

# Exercice 3 : Tri d'arguments
./exercice3/sortarg2 "zèbre" "abeille" "lion" "chat"

# Exercice 4-5 : s3tool
source ./exercice4-5/s3tool.sh
s36str "cc" "bb" "aa"
./exercice4-5/s3s "chat" "abeille" "zèbre"
./exercice4-5/s3e 30 10 20
```

### TP4 - Tests des programmes C

```bash
cd ISE_TP4

# Compiler tout
make

# Tester rapidement
make test

# Exercice 1 : Signaux (nécessite 2 terminaux)
# Terminal 1
./exercice1/sigcatch

# Terminal 2
./exercice1/sigsend USR1 <PID_du_sigcatch>
./exercice1/sigsend INT <PID>

# Exercice 2 : mycat
echo "test" | ./exercice2/mycat
./exercice2/mycat < /etc/passwd

# Exercice 3 : Taille des pipes
./exercice3/pipe_size_fcntl

# Exercice 4 : mysleep
./exercice4/mysleep 5
```

## 📚 CONCEPTS FONDAMENTAUX

### 🔄 Processus : comprendre l'exécution des programmes

**Qu'est-ce qu'un processus ?**
Un processus est un programme en cours d'exécution. Chaque processus a :
- Un **PID** (Process ID) : numéro unique pour l'identifier
- Un **PPID** (Parent PID) : PID du processus qui l'a créé
- Un **état** : running, sleeping, stopped, zombie
- Des **ressources** : mémoire, fichiers ouverts, variables

**États d'un processus :**
```
┌─────────────┐
│   CREATED   │  Processus créé (fork)
└──────┬──────┘
       ↓
┌─────────────┐
│   READY     │  Prêt à s'exécuter
└──────┬──────┘
       ↓
┌─────────────┐
│   RUNNING   │  En cours d'exécution (code s'exécute)
└──────┬──────┘
       ↓
┌─────────────┐
│  WAITING    │  Attend une ressource (I/O, signal...)
│ (Sleeping)  │
└──────┬──────┘
       ↓
┌─────────────┐
│  STOPPED    │  Mis en pause (SIGSTOP, Ctrl-Z)
└──────┬──────┘
       ↓
┌─────────────┐
│ TERMINATED  │  Terminé mais pas encore nettoyé
│  (Zombie)   │
└─────────────┘
```

**Avant-plan vs Arrière-plan :**
```bash
# Avant-plan (foreground) : bloque le terminal
./programme              # Terminal occupé jusqu'à la fin

# Arrière-plan (background) : terminal libre
./programme &            # Terminal utilisable, programme continue

# Contrôle :
Ctrl-Z                   # Suspendre le programme en avant-plan (SIGTSTP)
jobs                     # Lister les processus en arrière-plan
fg                       # Ramener en avant-plan
fg %2                    # Ramener le job numéro 2 en avant-plan
bg                       # Continuer en arrière-plan
bg %2                    # Continuer le job 2 en arrière-plan
```

**Exemple pratique :**
```bash
# Lancer un éditeur qui bloque le terminal
gedit fichier.txt        # Terminal bloqué ❌

# Mieux : lancer en arrière-plan
gedit fichier.txt &      # Terminal libre ✓

# Ou si déjà lancé :
# 1. Ctrl-Z pour suspendre
# 2. bg pour continuer en arrière-plan
```

### 📊 Flux (Streams) : comment les données circulent

**Les 3 flux standards :**
```
Programme
    ↑
    │ stdin (0)   ← Entrée standard (clavier par défaut)
    │
    ├─ stdout (1) → Sortie standard (écran par défaut)
    │
    └─ stderr (2) → Sortie d'erreur (écran par défaut)
```

**Exemple concret :**
```c
scanf("%d", &x);          // Lit depuis stdin (fd 0)
printf("Résultat: %d", x); // Écrit sur stdout (fd 1)
fprintf(stderr, "Erreur!"); // Écrit sur stderr (fd 2)
```

**Redirections : changer où vont les données**
```bash
# Rediriger stdout vers un fichier
./programme > sortie.txt           # Écraser le fichier
./programme >> sortie.txt          # Ajouter à la fin

# Rediriger stderr
./programme 2> erreurs.txt         # Seulement les erreurs

# Rediriger les deux
./programme > tout.txt 2>&1        # stdout et stderr dans même fichier
./programme &> tout.txt            # Syntaxe courte (bash)

# Rediriger stdin
./programme < entree.txt           # Lire depuis un fichier

# Combiner
./programme < input.txt > output.txt 2> erreurs.txt

# Ignorer les erreurs
./programme 2> /dev/null           # Jeter stderr
```

**Exemple complet :**
```bash
# Sans redirection
ls /home /inexistant
# Affiche à l'écran :
#   /home: (contenu)           ← stdout
#   ls: /inexistant: No such file ← stderr

# Avec redirections
ls /home /inexistant > fichiers.txt 2> erreurs.txt
# fichiers.txt contient le contenu de /home
# erreurs.txt contient le message d'erreur
```

### 🔗 Pipes : connecter des programmes

**Concept :** Le pipe `|` connecte la sortie d'un programme à l'entrée du suivant

```
programme1 | programme2 | programme3

stdout → stdin   stdout → stdin
   ↓        ↓        ↓        ↓
  [1]  →  [2]  →  [3]  →  écran
```

**Exemples progressifs :**
```bash
# 1. Simple : compter les lignes
cat fichier.txt | wc -l

# 2. Filtrer puis compter
cat /etc/passwd | grep "home" | wc -l

# 3. Extraire, trier, compter les doublons
cat fichier.txt | cut -d: -f1 | sort | uniq -c

# 4. Chaîne complexe
ps aux | grep firefox | awk '{print $2}' | head -n 1
# 1. Liste tous les processus
# 2. Garde seulement ceux contenant "firefox"
# 3. Extrait la 2e colonne (PID)
# 4. Garde la première ligne
```

**Différence pipe vs redirection :**
```bash
# Redirection : fichier
ls > liste.txt              # Écrit dans un fichier

# Pipe : autre programme
ls | grep ".txt"            # Passe à grep (en mémoire, pas de fichier)
```

### 🚰 FIFO (Named Pipes) : pipes avec un nom

**FIFO = First In First Out = pipe nommé**

Un FIFO est un fichier spécial qui agit comme un tuyau entre processus.

**Création :**
```bash
mkfifo /tmp/mon_pipe        # Créer un FIFO
ls -l /tmp/mon_pipe
# prw-r--r-- ... /tmp/mon_pipe    (le 'p' indique pipe)
```

**Utilisation :**
```bash
# Terminal 1 : Écrivain
echo "Bonjour" > /tmp/mon_pipe    # Bloque jusqu'à ce qu'un lecteur arrive

# Terminal 2 : Lecteur
cat < /tmp/mon_pipe               # Lit "Bonjour"
```

**Différence FIFO vs fichier régulier :**
```
Fichier régulier :
- Les données sont stockées sur le disque
- Plusieurs lecteurs lisent les MÊMES données
- Lecture non bloquante

FIFO :
- Les données transitent en mémoire
- Les lecteurs SE PARTAGENT les données (chaque octet lu une seule fois)
- Lecture bloquante si FIFO vide ET écrivain existe
- Écriture bloquante si FIFO pleine ET lecteur existe
```

**Exemple pratique :**
```bash
# Créer un FIFO
mkfifo /tmp/log_pipe

# Terminal 1 : Programme qui génère des logs
while true ; do
    echo "Log: $(date)" > /tmp/log_pipe
    sleep 1
done

# Terminal 2 : Programme qui traite les logs
while read line ; do
    echo "Reçu: $line"
done < /tmp/log_pipe

# Les logs circulent du terminal 1 au terminal 2 via le FIFO
```

### 📡 Communication inter-processus

**1. Signaux : messages asynchrones**

Les signaux sont comme des notifications envoyées aux processus.

```bash
# Envoyer un signal
kill -SIGNAL PID

# Signaux courants
kill -INT PID       # Interruption (comme Ctrl-C)
kill -TERM PID      # Terminaison propre
kill -KILL PID      # Terminaison forcée (non capturable)
kill -STOP PID      # Pause (non capturable)
kill -CONT PID      # Reprise
kill -USR1 PID      # Signal utilisateur 1
```

**Capturer un signal en C :**
```c
#include <signal.h>

void mon_handler(int sig) {
    printf("Signal %d reçu!\n", sig);
}

int main() {
    signal(SIGINT, mon_handler);  // Capturer Ctrl-C
    
    while(1) {
        printf("En attente...\n");
        sleep(1);
    }
}
```

**2. Pipes anonymes (entre processus apparentés)**

```c
int pipefd[2];
pipe(pipefd);  // Créer un pipe

// pipefd[0] : lecture
// pipefd[1] : écriture

if (fork() == 0) {
    // Processus enfant : écrire
    close(pipefd[0]);
    write(pipefd[1], "Hello", 5);
} else {
    // Processus parent : lire
    close(pipefd[1]);
    char buf[10];
    read(pipefd[0], buf, 5);
}
```

**3. FIFO (entre processus non apparentés)**
Voir section FIFO ci-dessus.

### 🎯 Descripteurs de fichiers (File Descriptors)

**Qu'est-ce qu'un fd ?**
Un nombre entier qui représente un fichier ouvert (ou pipe, socket...).

**Descripteurs standards :**
```
0 = STDIN_FILENO   (stdin)   Entrée
1 = STDOUT_FILENO  (stdout)  Sortie
2 = STDERR_FILENO  (stderr)  Erreurs
3, 4, 5... = fichiers ouverts par le programme
```

**Opérations sur les fd :**
```c
// Ouvrir
int fd = open("fichier.txt", O_RDONLY);  // Lecture seule
int fd = open("fichier.txt", O_WRONLY);  // Écriture seule
int fd = open("fichier.txt", O_RDWR);    // Lecture/écriture

// Lire
char buf[100];
ssize_t n = read(fd, buf, 100);  // Lit jusqu'à 100 octets

// Écrire
write(fd, "texte", 5);  // Écrit 5 octets

// Fermer
close(fd);
```

**Manipulation avec fcntl :**
```c
#include <fcntl.h>

// Obtenir la taille d'un pipe
int size = fcntl(pipefd[0], F_GETPIPE_SZ);

// Rendre un fd non-bloquant
int flags = fcntl(fd, F_GETFL);
fcntl(fd, F_SETFL, flags | O_NONBLOCK);
```

## 🛠️ Aide-mémoire des commandes

### Gestion des processus

```bash
# Lister les processus
ps                       # Processus de la session courante
ps aux                   # Tous les processus (détaillé)
ps -ef                   # Format différent
pstree                   # Arbre des processus
top                      # Monitoring en temps réel
htop                     # Version améliorée de top

# Informations sur un processus
ps -p PID                # Infos sur un PID spécifique
pgrep firefox            # Trouver le PID de firefox
pidof firefox            # Idem

# Envoyer des signaux
kill PID                 # SIGTERM (15) par défaut
kill -9 PID              # SIGKILL (forcé, non capturable)
kill -STOP PID           # Pause
kill -CONT PID           # Reprise
killall firefox          # Tuer tous les processus "firefox"
pkill firefox            # Idem

# Jobs (processus lancés depuis le terminal)
jobs                     # Lister les jobs
jobs -l                  # Avec les PIDs
fg                       # Ramener en avant-plan
fg %2                    # Ramener le job 2
bg                       # Continuer en arrière-plan
bg %2                    # Continuer le job 2
disown                   # Détacher du terminal
```

### Redirections et pipes

```bash
# Redirections de sortie
cmd > file               # Écraser file avec stdout
cmd >> file              # Ajouter à file
cmd 2> file              # Rediriger stderr
cmd &> file              # stdout et stderr
cmd > file 2>&1          # Idem (portable)
cmd > /dev/null 2>&1     # Tout ignorer

# Redirections d'entrée
cmd < file               # Lire depuis file
cmd << EOF               # Here document
texte
EOF

# Pipes
cmd1 | cmd2              # stdout de cmd1 → stdin de cmd2
cmd1 |& cmd2             # stdout et stderr de cmd1 → stdin de cmd2
cmd1 | tee file.txt      # Afficher ET sauvegarder

# FIFO
mkfifo pipe_name         # Créer un FIFO
rm pipe_name             # Supprimer un FIFO
```

### Traitement de texte

```bash
# Lecture
cat file                 # Afficher tout
less file                # Paginer (q pour quitter)
head -n 10 file          # 10 premières lignes
tail -n 10 file          # 10 dernières lignes
tail -f file             # Suivre les ajouts (logs)

# Recherche
grep "motif" file        # Lignes contenant "motif"
grep -v "motif" file     # Lignes NE contenant PAS "motif"
grep -i "motif" file     # Insensible à la casse
grep -r "motif" dir/     # Récursif dans un répertoire
grep -n "motif" file     # Avec numéros de ligne

# Découpage
cut -d: -f1 file         # 1ère colonne (délimiteur :)
cut -d: -f1,3 file       # Colonnes 1 et 3
awk '{print $2}' file    # 2e colonne (délimiteur espace)
awk -F: '{print $1}' file # 1ère colonne (délimiteur :)

# Transformation
sed 's/old/new/' file    # Remplacer 1ère occurrence par ligne
sed 's/old/new/g' file   # Remplacer toutes les occurrences
sed '/motif/d' file      # Supprimer les lignes avec "motif"
sed -n '5,10p' file      # Afficher lignes 5 à 10
tr 'a-z' 'A-Z'           # Minuscules → majuscules

# Tri et unicité
sort file                # Trier alphabétiquement
sort -n file             # Trier numériquement
sort -r file             # Trier en ordre inverse
uniq file                # Supprimer les doublons consécutifs
sort file | uniq         # Trier puis dédupliquer
sort file | uniq -c      # Compter les occurrences

# Comptage
wc file                  # Lignes, mots, octets
wc -l file               # Nombre de lignes
wc -w file               # Nombre de mots
wc -c file               # Nombre d'octets
```

### Navigation et fichiers

```bash
# Navigation
cd ~                     # Aller au HOME
cd -                     # Répertoire précédent
pwd                      # Répertoire courant
pushd /tmp               # Sauvegarder et aller à /tmp
popd                     # Retourner au répertoire sauvegardé

# Manipulation fichiers
cp -v src dest           # Copier (verbose)
cp -r dir1 dir2          # Copier récursif
mv -v src dest           # Déplacer/renommer
rm -v file               # Supprimer
rm -rf dir               # Supprimer récursif (attention!)
mkdir -p a/b/c           # Créer arborescence
touch file               # Créer fichier vide / mettre à jour date

# Recherche
find . -name "*.txt"     # Trouver par nom
find . -type f           # Trouver fichiers réguliers
find . -type d           # Trouver répertoires
find . -size +10M        # Fichiers > 10 Mo
find . -mtime -7         # Modifiés dans les 7 derniers jours

# Permissions
chmod +x file            # Ajouter droit d'exécution
chmod 755 file           # rwxr-xr-x
chmod -R 644 dir/        # Récursif
chown user:group file    # Changer propriétaire
```

### Variables et environnement

```bash
# Variables
var="valeur"             # Définir (PAS d'espace autour du =)
echo $var                # Afficher
echo "$var"              # Afficher (avec expansion)
echo '$var'              # Afficher littéralement : $var
unset var                # Supprimer

# Variables d'environnement
export VAR="valeur"      # Exporter (disponible pour sous-processus)
env                      # Lister toutes les variables d'environnement
echo $PATH               # Afficher PATH
export PATH="$PATH:dir"  # Ajouter au PATH

# Variables spéciales
$?                       # Code de retour dernière commande
$!                       # PID dernier processus en arrière-plan
$$                       # PID du shell courant
$0                       # Nom du script
$1, $2, ...              # Arguments du script
$#                       # Nombre d'arguments
$@                       # Tous les arguments
$*                       # Tous les arguments (différent de $@)
```

### Compilation C

```bash
# Compilation simple
gcc -o prog prog.c

# Avec options recommandées (TOUJOURS utiliser)
gcc -Wall -Wextra -std=c99 -o prog prog.c
# -Wall : Active tous les warnings
# -Wextra : Warnings supplémentaires
# -std=c99 : Standard C99

# Avec bibliothèque math
gcc -Wall -Wextra -std=c99 -o prog prog.c -lm
# -lm doit être À LA FIN

# Avec définitions GNU (pour F_GETPIPE_SZ, etc.)
gcc -Wall -Wextra -std=c99 -D_GNU_SOURCE -o prog prog.c

# Debugging
gcc -g -Wall -Wextra -o prog prog.c    # Ajouter symboles debug
gdb ./prog                              # Lancer debugger

# Vérification mémoire
valgrind --leak-check=full ./prog      # Détecter fuites mémoire
valgrind --track-origins=yes ./prog    # Tracer variables non initialisées
```

### Scripts Shell

```bash
# Exécuter un script
bash script.sh           # Avec bash
sh script.sh             # Avec sh (plus portable)
./script.sh              # Direct (nécessite chmod +x)

# Rendre exécutable
chmod +x script.sh

# Shebang (première ligne du script)
#!/bin/bash              # Pour bash
#!/bin/sh                # Pour sh (portable)
#!/usr/bin/env bash      # Trouve bash dans PATH

# Déboguer un script
bash -x script.sh        # Affiche chaque commande exécutée
set -x                   # Dans le script : activer mode debug
set +x                   # Désactiver mode debug
set -e                   # Arrêter sur première erreur
set -u                   # Erreur si variable non définie

# Variables dans scripts
$1, $2, ...              # Arguments
$#                       # Nombre d'arguments
$@                       # Tous les arguments (liste)
$*                       # Tous les arguments (chaîne)
$0                       # Nom du script
$?                       # Code retour dernière commande

# Tests conditionnels
[ -f file ]              # Fichier régulier existe
[ -d dir ]               # Répertoire existe
[ -e path ]              # Chemin existe
[ -r file ]              # Fichier lisible
[ -w file ]              # Fichier modifiable
[ -x file ]              # Fichier exécutable
[ -z "$var" ]            # Variable vide
[ -n "$var" ]            # Variable non vide
[ "$a" = "$b" ]          # Égalité chaînes
[ "$a" != "$b" ]         # Différence chaînes
[ $a -eq $b ]            # Égalité nombres
[ $a -ne $b ]            # Différence nombres
[ $a -lt $b ]            # Plus petit que
[ $a -le $b ]            # Plus petit ou égal
[ $a -gt $b ]            # Plus grand que
[ $a -ge $b ]            # Plus grand ou égal

# Boucles
while [ condition ] ; do
    commandes
done

for var in liste ; do
    commandes
done

for ((i=0; i<10; i++)) ; do
    commandes
done

# Fonctions
ma_fonction() {
    local var="valeur"   # Variable locale
    echo "$1"            # Premier argument
    return 0             # Code retour
}

# Appel
ma_fonction arg1 arg2
```

### 📋 Table complète des signaux

| Signal   | Numéro | Raccourci | Description                    | Capturable | Usage typique |
|----------|--------|-----------|--------------------------------|------------|---------------|
| SIGHUP   | 1      | -         | Hangup (déconnexion terminal)  | Oui        | Recharger config |
| SIGINT   | 2      | Ctrl-C    | Interruption utilisateur       | Oui        | Arrêt propre |
| SIGQUIT  | 3      | Ctrl-\\   | Quit + core dump               | Oui        | Debug crash |
| SIGILL   | 4      | -         | Instruction illégale           | Oui        | Erreur CPU |
| SIGTRAP  | 5      | -         | Breakpoint (debug)             | Oui        | Debugger |
| SIGABRT  | 6      | -         | Abort (assert, abort())        | Oui        | Erreur fatale |
| SIGBUS   | 7      | -         | Bus error (alignement mémoire) | Oui        | Erreur hardware |
| SIGFPE   | 8      | -         | Floating point exception       | Oui        | Division par 0 |
| **SIGKILL** | **9** | -     | **Terminaison FORCÉE**         | **NON**    | **Tuer processus** |
| SIGUSR1  | 10     | -         | Signal utilisateur 1           | Oui        | IPC custom |
| SIGSEGV  | 11     | -         | Segmentation fault             | Oui        | Accès mémoire illégal |
| SIGUSR2  | 12     | -         | Signal utilisateur 2           | Oui        | IPC custom |
| SIGPIPE  | 13     | -         | Write to pipe sans lecteur     | Oui        | Broken pipe |
| SIGALRM  | 14     | -         | Alarme (alarm(), setitimer())  | Oui        | Timeout |
| SIGTERM  | 15     | -         | Terminaison propre             | Oui        | Arrêt gracieux |
| SIGCHLD  | 17     | -         | Enfant terminé/stoppé          | Oui        | Wait() |
| SIGCONT  | 18     | -         | Continue (si stoppé)           | Oui        | Reprise |
| **SIGSTOP** | **19** | -     | **Stop FORCÉ**                 | **NON**    | **Pause processus** |
| SIGTSTP  | 20     | Ctrl-Z    | Stop terminal (capturable)     | Oui        | Pause utilisateur |
| SIGTTIN  | 21     | -         | Read depuis terminal (bg)      | Oui        | Contrôle terminal |
| SIGTTOU  | 22     | -         | Write vers terminal (bg)       | Oui        | Contrôle terminal |

**Signaux NON CAPTURABLES (⚠️ très important) :**
- **SIGKILL (9)** : Tue IMMÉDIATEMENT, pas de nettoyage possible
- **SIGSTOP (19)** : Pause IMMÉDIATE, pas de handler possible

**Utilisation en Shell :**
```bash
# Par numéro
kill -9 PID              # SIGKILL
kill -15 PID             # SIGTERM (défaut)

# Par nom (sans SIG)
kill -KILL PID           # SIGKILL
kill -TERM PID           # SIGTERM
kill -INT PID            # SIGINT
kill -STOP PID           # SIGSTOP
kill -CONT PID           # SIGCONT

# Exemples pratiques
kill PID                 # Demande proprement (SIGTERM)
kill -9 PID              # Force (SIGKILL) si le processus ne répond pas
kill -STOP PID           # Met en pause
kill -CONT PID           # Reprend
```

**Utilisation en C :**
```c
#include <signal.h>

// Capturer un signal
void handler(int sig) {
    printf("Signal %d reçu\n", sig);
}

signal(SIGINT, handler);      // Capturer Ctrl-C
signal(SIGTERM, handler);     // Capturer kill
signal(SIGUSR1, handler);     // Signal custom

// Ignorer un signal
signal(SIGPIPE, SIG_IGN);     // Ignorer SIGPIPE

// Comportement par défaut
signal(SIGINT, SIG_DFL);      // Réinitialiser

// Envoyer un signal
kill(pid, SIGUSR1);           // Envoyer SIGUSR1 au processus pid
raise(SIGTERM);               // S'envoyer SIGTERM à soi-même

// Attendre un signal
pause();                      // Bloque jusqu'à réception d'un signal

// Programmer une alarme
alarm(5);                     // SIGALRM dans 5 secondes
```

**Comportements par défaut :**
- **Term** : Terminer le processus
- **Ign** : Ignorer le signal
- **Core** : Terminer + créer core dump
- **Stop** : Arrêter (pause)
- **Cont** : Continuer (si arrêté)

## 🔧 Dépannage et erreurs courantes

### Problèmes Shell

**1. Permission denied lors de l'exécution**
```bash
$ ./script.sh
bash: ./script.sh: Permission denied

# Solution :
chmod +x script.sh
```

**2. Script non trouvé**
```bash
$ script.sh
bash: script.sh: command not found

# Solutions :
./script.sh                    # Chemin relatif
/home/user/script.sh           # Chemin absolu
export PATH="$PWD:$PATH"       # Ajouter au PATH (temporaire)
```

**3. Bad interpreter**
```bash
$ ./script.sh
bash: ./script.sh: /bin/bash^M: bad interpreter

# Cause : Fichier créé sur Windows (CRLF au lieu de LF)
# Solution :
dos2unix script.sh             # Convertir
sed -i 's/\r$//' script.sh     # Ou avec sed
```

**4. Variable non définie**
```bash
$ echo $VARIABLE

# Rien ne s'affiche
# Vérifier :
set | grep VARIABLE            # Lister toutes les variables
env | grep VARIABLE            # Variables d'environnement seulement
```

**5. Substitution de commande ne fonctionne pas**
```bash
# ❌ Mauvais
result='$(ls)'                 # Guillemets simples : pas d'expansion
echo $result                   # Affiche : $(ls)

# ✓ Bon
result=$(ls)                   # Ou : result=`ls`
echo "$result"                 # Affiche le résultat de ls
```

### Problèmes Processus et Signaux

**6. Processus zombie**
```bash
$ ps aux | grep defunct
user  1234  0.0  0.0   0   0 ?  Z  10:00  0:00 [prog] <defunct>

# Cause : Processus terminé mais parent n'a pas fait wait()
# Solution : Tuer le parent
kill PPID
```

**7. Processus ne répond plus**
```bash
# Tenter terminaison propre
kill PID
kill -TERM PID

# Si ça ne marche pas, forcer
kill -9 PID
kill -KILL PID

# Tuer tous les processus d'un nom
killall firefox
pkill firefox
```

**8. Too many open files**
```bash
$ ./prog
Error: Too many open files

# Voir la limite
ulimit -n                      # Nombre max de fd

# Augmenter (temporaire)
ulimit -n 4096

# Vérifier les fd ouverts d'un processus
ls -l /proc/PID/fd
lsof -p PID
```

### Problèmes Pipes et FIFO

**9. Pipe bloqué**
```bash
# Symptôme : Programme ne fait rien, ne répond pas

# Diagnostic :
ps aux | grep mycat            # Voir si processus existe
lsof /tmp/myfifo               # Voir qui utilise le FIFO

# Solutions :
kill -9 PID                    # Tuer le processus
rm /tmp/myfifo                 # Supprimer le FIFO
mkfifo /tmp/myfifo             # Recréer
```

**10. Broken pipe**
```bash
$ ./prog > /tmp/myfifo
Broken pipe

# Cause : Pas de lecteur sur le FIFO
# Solution : Lancer un lecteur d'abord
cat < /tmp/myfifo &            # Lecteur en arrière-plan
./prog > /tmp/myfifo           # Puis écrivain
```

**11. FIFO ne transmet pas les données**
```bash
# Vérifier le type du fichier
ls -l /tmp/myfifo
# Doit afficher : prw-r--r-- (le 'p' indique pipe)

# Si c'est un fichier régulier 'f' :
rm /tmp/myfifo
mkfifo /tmp/myfifo
```

### Problèmes Compilation C

**12. Erreur de compilation**
```bash
$ gcc prog.c
prog.c:10:5: error: 'x' undeclared

# Vérifier syntaxe seulement
gcc -Wall -Wextra -fsyntax-only prog.c

# Compiler avec détails
gcc -Wall -Wextra -std=c99 -v prog.c 2>&1 | less
```

**13. Undefined reference**
```bash
$ gcc prog.c
undefined reference to `sqrt'

# Cause : Oublié de lier la bibliothèque math
# Solution :
gcc prog.c -lm                 # -lm À LA FIN
```

**14. Segmentation fault**
```bash
$ ./prog
Segmentation fault

# Debugger avec gdb
gcc -g prog.c -o prog          # Compiler avec symboles debug
gdb ./prog
(gdb) run                      # Exécuter
(gdb) backtrace                # Voir la pile d'appels
(gdb) quit

# Ou avec valgrind
valgrind ./prog                # Détecte erreurs mémoire
```

**15. Warning: format '%d' expects argument of type 'int'**
```c
// ❌ Mauvais
size_t n = 10;
printf("%d\n", n);             // size_t n'est pas int

// ✓ Bon
printf("%zu\n", n);            // %zu pour size_t
printf("%ld\n", (long)n);      // Ou cast en long
```

### Problèmes Redirections

**16. Redirection n'écrit rien**
```bash
$ ls > fichier.txt 2> erreurs.txt
$ cat erreurs.txt
# Vide

# Vérifier que stderr a bien des erreurs
ls /inexistant > fichier.txt 2> erreurs.txt
cat erreurs.txt                # Maintenant contient l'erreur
```

**17. Fichier écrasé par erreur**
```bash
$ cat important.txt > important.txt
# Fichier vidé ! ❌

# Prévention : option noclobber
set -o noclobber               # Empêche écrasement avec >
# Utiliser >| pour forcer

# Toujours utiliser >> pour ajouter
cat new.txt >> important.txt   # Ajoute sans écraser
```

**18. Redirection ne marche pas dans un script**
```bash
#!/bin/bash
echo "test" > fichier.txt      # Ne crée rien

# Vérifier :
bash -x script.sh              # Mode debug
# Vérifier les permissions du répertoire
ls -ld .                       # Droit d'écriture ?
```

### Astuces de débogage

**Activer le mode trace**
```bash
# Shell
set -x                         # Afficher chaque commande
set -v                         # Afficher chaque ligne lue
set -e                         # Arrêter sur première erreur

# C
gcc -g prog.c                  # Symboles de debug
gdb ./prog                     # Debugger
valgrind ./prog                # Vérificateur mémoire
strace ./prog                  # Tracer appels système
```

**Vérifier les valeurs**
```bash
# Shell
echo "var=$var"                # Afficher variable
echo "args=$@"                 # Afficher arguments
echo "retour=$?"               # Code retour

# C
printf("debug: x=%d\n", x);    # Debug print
fprintf(stderr, "x=%d\n", x);  # Sur stderr
```

**Logs**
```bash
# Rediriger tout dans un log
./prog > log.txt 2>&1

# Voir en temps réel
tail -f log.txt

# Dans le programme
./prog 2>&1 | tee log.txt      # Affiche ET sauvegarde
```

## 📝 Exemples pratiques complets

### Exemple 1 : Pipeline de traitement de logs

**Objectif** : Analyser un fichier de logs pour trouver les IPs les plus actives

```bash
# Fichier access.log contenant :
# 192.168.1.10 - - [17/Nov/2025:10:00:00] "GET /page.html"
# 192.168.1.20 - - [17/Nov/2025:10:01:00] "POST /api"
# 192.168.1.10 - - [17/Nov/2025:10:02:00] "GET /index.html"

# Extraire les IPs, trier, compter, afficher top 10
cat access.log | \                    # Lire le fichier
    awk '{print $1}' | \              # Extraire 1ère colonne (IP)
    sort | \                          # Trier
    uniq -c | \                       # Compter occurrences
    sort -rn | \                      # Trier numériquement (décroissant)
    head -n 10                        # Top 10

# Résultat :
#   5 192.168.1.10
#   3 192.168.1.20
#   2 192.168.1.30
```

### Exemple 2 : Communication entre processus avec FIFO

**Programme 1 : Producteur (générateur de données)**
```c
// producer.c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("/tmp/myfifo", O_WRONLY);  // Ouvrir FIFO en écriture
    
    for (int i = 0; i < 10; i++) {
        char msg[50];
        snprintf(msg, sizeof(msg), "Message %d\n", i);
        write(fd, msg, strlen(msg));
        sleep(1);
    }
    
    close(fd);
    return 0;
}
```

**Programme 2 : Consommateur (traitement des données)**
```c
// consumer.c
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("/tmp/myfifo", O_RDONLY);  // Ouvrir FIFO en lecture
    
    char buf[100];
    ssize_t n;
    
    while ((n = read(fd, buf, sizeof(buf)-1)) > 0) {
        buf[n] = '\0';
        printf("Reçu : %s", buf);
    }
    
    close(fd);
    return 0;
}
```

**Utilisation :**
```bash
# Créer le FIFO
mkfifo /tmp/myfifo

# Compiler
gcc -o producer producer.c
gcc -o consumer consumer.c

# Terminal 1
./consumer

# Terminal 2
./producer

# Le consumer affiche les messages au fur et à mesure
```

### Exemple 3 : Script avec gestion d'erreurs robuste

```bash
#!/bin/bash
# backup.sh - Script de sauvegarde robuste

# Arrêter sur erreur
set -e
set -u
set -o pipefail

# Variables
SOURCE="/home/user/documents"
DEST="/backup"
DATE=$(date +%Y%m%d_%H%M%S)
LOGFILE="/var/log/backup_${DATE}.log"

# Fonction de log
log() {
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] $*" | tee -a "$LOGFILE"
}

# Fonction d'erreur
error() {
    log "ERREUR: $*" >&2
    exit 1
}

# Vérifications
[ -d "$SOURCE" ] || error "Source $SOURCE n'existe pas"
[ -d "$DEST" ] || error "Destination $DEST n'existe pas"

# Sauvegarde
log "Début de la sauvegarde"
if tar czf "${DEST}/backup_${DATE}.tar.gz" "$SOURCE" 2>&1 | tee -a "$LOGFILE" ; then
    log "Sauvegarde réussie : backup_${DATE}.tar.gz"
else
    error "Échec de la sauvegarde"
fi

# Nettoyage des anciennes sauvegardes (garder 7 jours)
log "Nettoyage des anciennes sauvegardes"
find "$DEST" -name "backup_*.tar.gz" -mtime +7 -delete

log "Terminé"
```

### Exemple 4 : Surveillance de processus avec signaux

```c
// monitor.c - Surveille un processus et réagit aux signaux
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile int should_stop = 0;
volatile int pause_monitoring = 0;

void sigint_handler(int sig) {
    printf("\nArrêt demandé (Ctrl-C)...\n");
    should_stop = 1;
}

void sigusr1_handler(int sig) {
    pause_monitoring = !pause_monitoring;
    printf("\nMonitoring %s\n", pause_monitoring ? "en PAUSE" : "ACTIF");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        return 1;
    }
    
    pid_t target = atoi(argv[1]);
    
    // Installer les gestionnaires
    signal(SIGINT, sigint_handler);
    signal(SIGUSR1, sigusr1_handler);
    
    printf("Monitoring du processus %d\n", target);
    printf("Ctrl-C pour arrêter, SIGUSR1 pour pause/reprise\n");
    printf("Commande : kill -USR1 %d\n\n", getpid());
    
    while (!should_stop) {
        if (!pause_monitoring) {
            // Vérifier si le processus existe
            if (kill(target, 0) == 0) {
                printf("Processus %d : ACTIF\r", target);
                fflush(stdout);
            } else {
                printf("\nProcessus %d : TERMINÉ\n", target);
                break;
            }
        }
        sleep(1);
    }
    
    printf("\nMonitoring terminé\n");
    return 0;
}
```

**Utilisation :**
```bash
gcc -o monitor monitor.c

# Terminal 1 : Lancer un processus long
sleep 100 &
PID=$!

# Terminal 2 : Surveiller
./monitor $PID

# Terminal 3 : Mettre en pause
kill -USR1 $(pgrep monitor)

# Arrêter
kill -INT $(pgrep monitor)
# ou Ctrl-C dans le terminal du monitor
```

### Exemple 5 : Traitement parallèle avec pipes

```bash
#!/bin/bash
# parallel_process.sh - Traiter un gros fichier en parallèle

INPUT="big_file.txt"
TEMP_DIR="/tmp/parallel_$$"
NUM_JOBS=4

mkdir -p "$TEMP_DIR"

# Diviser le fichier en morceaux
split -n l/$NUM_JOBS "$INPUT" "$TEMP_DIR/chunk_"

# Traiter chaque morceau en parallèle
for chunk in "$TEMP_DIR"/chunk_* ; do
    (
        # Traitement (exemple : compter les mots)
        wc -w < "$chunk" > "${chunk}.result"
    ) &  # Lancer en arrière-plan
done

# Attendre que tous les jobs se terminent
wait

# Combiner les résultats
total=0
for result in "$TEMP_DIR"/*.result ; do
    count=$(cat "$result")
    total=$((total + count))
done

echo "Total de mots : $total"

# Nettoyage
rm -rf "$TEMP_DIR"
```

## 🎓 Mémo récapitulatif pour examens

### Commandes à connaître PAR CŒUR

```bash
# Processus
ps aux                   # Lister tous les processus
kill -9 PID              # Tuer un processus
jobs                     # Lister jobs
fg / bg                  # Avant/arrière-plan

# Redirections
cmd > file               # Stdout vers fichier
cmd 2> file              # Stderr vers fichier
cmd &> file              # Tout vers fichier
cmd | cmd2               # Pipe

# FIFO
mkfifo pipe              # Créer
rm pipe                  # Supprimer

# Signaux
kill -TERM PID           # Terminaison propre
kill -KILL PID           # Terminaison forcée
kill -STOP PID           # Pause
kill -CONT PID           # Reprise

# Texte
grep "motif" file        # Recherche
cut -d: -f1 file         # Extraction
sort | uniq -c           # Trier et compter
head / tail              # Début / fin
```

### Fonctions C à connaître

```c
// Signaux
signal(SIGINT, handler); // Capturer
kill(pid, SIGUSR1);      // Envoyer

// Processus
fork();                  // Créer processus enfant
wait(NULL);              // Attendre enfant
exit(0);                 // Terminer

// Pipes
pipe(fd);                // Créer pipe
read(fd, buf, n);        // Lire
write(fd, buf, n);       // Écrire
close(fd);               // Fermer

// FIFO
mkfifo(path, mode);      // Créer FIFO
open(path, O_RDONLY);    // Ouvrir
unlink(path);            // Supprimer

// Temps
alarm(seconds);          // Alarme
pause();                 // Attendre signal
sleep(seconds);          // Dormir
```

### Pièges à éviter

❌ **NE PAS FAIRE :**
```bash
cat file | grep motif     # Inutile, grep peut lire directement
grep motif < file         # Mieux

cmd > file ; cmd2 > file  # Le 2e écrase
cmd > file ; cmd2 >> file # Mieux : append

kill -9 PID               # En premier recours
kill PID                  # D'abord essayer proprement
```

❌ **EN C :**
```c
signal(SIGKILL, handler); // IMPOSSIBLE (non capturable)
signal(SIGSTOP, handler); // IMPOSSIBLE

char *buf;
read(fd, buf, 100);       // buf non alloué ! Segfault
```

✓ **TOUJOURS :**
```bash
# Vérifier les codes retour
if command ; then
    echo "Succès"
else
    echo "Échec"
fi

# Quoter les variables
rm "$fichier"             # Pas : rm $fichier
```

✓ **EN C :**
```c
// Vérifier les retours
if (pipe(fd) < 0) {
    perror("pipe");
    exit(1);
}

// Fermer les fd inutiles
close(fd[0]);  // Si on n'écrit pas
close(fd[1]);  // Si on ne lit pas
```

## Ressources et documentation

### Manuels en ligne
```bash
man bash                 # Manuel de bash
man signal               # Page sur les signaux
man pipe                 # Documentation pipe()
man kill                 # Commande kill
info coreutils           # Utilitaires GNU

# Chercher dans les man pages
man -k pipe              # Toutes les pages contenant "pipe"
apropos signal           # Idem
```

### Liens utiles
- [Bash Guide](https://mywiki.wooledge.org/BashGuide)
- [Advanced Bash Scripting Guide](https://tldp.org/LDP/abs/html/)
- [GNU Coreutils Manual](https://www.gnu.org/software/coreutils/manual/)
- [Linux Signal Man Page](https://man7.org/linux/man-pages/man7/signal.7.html)
- [POSIX Pipes](https://pubs.opengroup.org/onlinepubs/9699919799/functions/pipe.html)

### Commandes d'aide rapide
```bash
help cd                  # Aide sur commandes built-in
type command             # Type de commande
which command            # Chemin de la commande
whereis command          # Toutes les localisations
```

## Auteur

TPs ISE - ENSIIE FISA IAP  
Année 2025

---

**Note importante :** Ce guide contient TOUT ce qu'il faut savoir pour les TPs ISE. Prenez le temps de lire les sections "Concepts fondamentaux" pour bien comprendre les processus, flux, pipes et signaux. Les exemples pratiques montrent comment utiliser ces concepts dans des cas réels.

Pour toute question ou problème, consultez :
1. Les sections de dépannage ci-dessus
2. Les README.md dans chaque sous-dossier pour les exercices spécifiques
3. Les commentaires détaillés dans les fichiers sources