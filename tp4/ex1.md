## 2

main
    lignes 27 à 32 Attache le gestionnnaire callback à tous les signaux de 1 à 22
    lignes 29 à 30 Affiche un message d'erreur pour les signaux qui ne sont pas attachables

    ligne 36 à 39 Boucle infinie : affichage d'un charactère puis attente d'un signal
callback (gestionnaire de signal)
    ligne 15 à 19 Affiche le PID et le signal attrapé

## 3.c

Ils écoutent tous les deux

## 7
b. oui, il se stop
c. SIGUSR1 ne fonctionne pas, il faut mettre le numéro équivalent (10) et celle-ci ne reçoit pas car il s'est stopé
d. Pareil que pour siguser1
e. il arrête de se stoper et continue | SIGCONT = 18
f. elles fonctionnent


## 8
On peut faire la commande deux fois, à la deuxième le programme est kill