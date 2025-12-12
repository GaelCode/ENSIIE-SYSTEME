## 1

**lignes 24 + 13 à 19**
Attache le gestionnaire au signal SIGPIPE. Le gestionnaire affiche un message, attend 5 secondes, puis termine le processus avec 0 comme code de retour.

**lignes 26 à 38** boucle infinie :
    - **ligne 28** Lecture d’un caractère c du flux d'entrée stadard;
    - **ligne 29 à 31** Si une erreure de lecture s’est produite, on écrit un message d’erreur et on termine le processus ;
    - **ligne 32 à 34** Si la fin du flux d’entrée est détectée, on écrit un "." sur le flux de sortie standard d'erreur, puis on attend 0.1 seconde ;
    - **ligne 36** Dans les autres cas, on écrit le caractère c sur le flux flux de sortie standard

## 3.d
Soit P1 et P2 deux processus, et f fichier régulier :
- Une lecture sur f n’est pas bloquante. Si on est en fin de fichier, le noyau renvoie une valeur
indiquant la fin de fichier.
- Si P1 et P2 écrivent dans f à la même position, la donnée de f à cette position sera celle de la dernière écriture.
- Si P1 et P2 lisent dans f à la même position, les données lues seront les mêmes si il n’y a
pas eu d'écriture dans f à cette position entre les deux lectures
- Si P1 fait une lecture et P2 une écriture dans f à la position `, P1 reçoit la donnée écrite
par P2 si :
1. la lecture de P1 est faite après l’écriture de P2,
2. il n’y a pas eu d’autre écriture entre la lecture de P1 et l’écriture de P2.

## 4.h

Soit P1 et P2 deux processus et f un fichier FIFO :
- Une lecture sur f est bloquante, si f est vide et qu’il existe encore un écrivain
- Une écriture sur f est bloquante, si f est pleine et qu’il existe encore un lecteur
- En absence de lecteur, une écriture sur une f génère l’émission du signal SIGPIPE. Si ce signal n’est ni attrapé, ni ignoré, ceci entraine la termninaison du processus. Si ce signal est attrapé ou ignoré, l’écriture renvoie une -1 et la variable errno reçoit la valeur EPIPE.
- Si P1 et P2 écrivent dans f , les données seront écrites soit l’une derrière l’autre, soit enchevêtrées.
- Si P1 et P2 lisent dans f , ils ne peuvent pas lire la même donnée.
- Si P1 fait une lecture et P2 une écriture dans f , P1 reçoit la donnée écrite par P2 si :
1. la FIFO f est vide,
2. il n’y a pas d’autre écriture entre la lecture de P1 et l’écriture de P2,
3. il n’y a pas d’autre lecture entre la lecture de P1 et l’écriture de P2.
Si ces conditions sont réalisées, l’ordre chronologique de la lecture de P1 et l’écriture de P2 n'a pas d'importance
