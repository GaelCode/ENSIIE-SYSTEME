# Notes script

## comment éxécuter un script :

- bash [nomfichier]
- ./[nomfichier]
- bash < [nomfichier]


--------------

## exemple type d'un fichier :
```bash
#!/bin/bash
echo -n "  " Hello
echo World
```

## Dans shell on peut sourcer un script (permet modifier les données du Shell (ex: PATH)):
- source [nomfichier]
- .[nomfichier]

-------------

## Syntaxe :

### else pas obligatoire
```bash
if cmd-cond ; then
    ....
elif cmd-cond2 ; then
    ....
else
    ....
fi
```
#### exemple:

```bash
if date | grep -q 2016 ; then
    echo oui
fi
```

### case
```bash
case mot inf 
    motif )
        ...
        ;;
    motif | motif )
        ...
        ;;
    ...
esac
```

## while, si while et do sur la même ligne mettre ; entre | break pour mettre fin à la boucle | continue pour revenir au début de la boucle

```bash
while condition
do 
    ...
done
```

### for
```bash
for v in str-list
do
    ...
done
```

#### exemple
```bash
for bn in $(cd $dir ; ls) do
    echo -n "$bn(" $(wc -l < $dir/$bn 2> /dev/null)"
done
echo
```

## Commande test

- **-n str** : *str est non vide*
- **-z str** : *str est vide*
- **str1 = str2** : *str1 et str2 sont égales (!= pour différentes)*
- **str1 \\< str2** : *str1 est inférieure (ordre lexicographique) à str2 (\> pour supérieure)*
- **n1 OP n2** : *l'entier n1 est OP à l'entier avec OP dans {-eq, -ne, -lt, -le, gt, or -ge} (égal, différent, inférieur, inférieur, inférieur ou égal, supérieur, supérieur ou égal)*
- **-e file** : *le fichier file existe et n'est pas un lien mort*
- **-f file** : *le fichier file existe et est régulier*
- **-d file** : *le fichier file existe et est un répertoire*
- **-h file** : *le fichier file existe et est lien symbolique*
- **-r file** : *le fichier file existe et peut être lu (file peut être un répertoire)*
- **-w file** : *le fichier file existe et peut être exécuté*
- **!expr** : *pour la négation*

Pour les chaînes de caractères on protège bien avec les guillemens pour les espaces : ``` "$str```

## Exécuter commande interne

On fait :
```bash
exec [commande(ex: cd)] arg1 arg2...
```

### Comment récupérer les arguments :

sh> ./script *petit* *lapin*


- taille= **$1** (premier argument) ---> *petit*
- animal= **$2** (deuxième argument) ---> *lapin*
- **$i** (ième argument)
- **$\*** (liste des arguments séparés par un espace) ---> arg1 arg2 ...
- **$@** (liste des arguments quotés séparés par des espaces) ---> "arg1" "arg2" ...
- **$0** (nom de la commande)
- **$#** (nombre d'argument)


#### exemple (script somme n1 n2 ...)
```bash
#!/bin/bash
first=1
for n in $* ; do
    test $first = 0 && echo -n + 
    echo -n $n # -n pas de saut de ligne
    first=0;
done
```

### Set

set -- str1 str2 str3 ... strm
-> affecte stri à $i pour i inférieur à n ==> j'ai pas compris

### ecp

```bash
ecp str1 str2 # copie str1 dans str2
```

### exit

**exit** ---> finir script

### read

read v1 v2 ...

v1, v2 ...sont des variables dans lesquelles on va mettre ce qu'on lit le read
Soit s1 s2 s3 le contenu d'une ligne

read
read v1 : v1 = s1 s2 s3
read v1 v2 : v1 = s1      v2= s2 s3
read v1 v2 v3 v4 : v1 = s1      v2= s2      v3 = s3     v4 = ""

#### Petit exemple

```bash
while read ligne ; do
    echo -n "$ligne "
done < /tmp/2
```

## deboguer

```bash
bash -x commande arg1 arg2 ... # activé degoguage
```