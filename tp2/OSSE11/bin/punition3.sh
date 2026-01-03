#!/bin/sh

# Vérification : on attend exactement 3 arguments
if [ $# -ne 3 ] ; then
    echo "usage: $0 <n_lignes> <m_mots> <mot>" >&2
    exit 1
fi

n="$1"
m="$2"
word="$3"

str=""
i=0
# On construit la ligne contenant le mot répété M fois
while [ $i -lt "$m" ] ; do
    str="$str $word"
    i=$((i+1))
done

# On appelle punition1.sh avec la ligne construite
# Note : on utilise ./ pour être sûr de pointer le script local
sh ./punition1.sh "$n" "$str"
