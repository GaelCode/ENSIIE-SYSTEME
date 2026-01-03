#!/bin/sh
#
#usage: punition1.sh n word

if [ $# -eq 0 ] ; then
    n=10
    m=3
    word="Defaut"
elif [ $# -eq 1 ] ; then
    n=10
    m=3
    word="$1"
elif [ $# -eq 2 ] ; then
    n=10
    m="$1"
    word="$2"
elif [ $# -eq 3 ] ; then
    n="$1"
    m="$2"
    word="$3"
else
    echo "usage $0 <n ou vide> <m ou vide> <word ou vide>"
fi

sh punition3.sh "$n" "$m" "$word"

