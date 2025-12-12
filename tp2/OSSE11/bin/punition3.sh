#!/bin/sh
#
#usage: punition1.sh n word

if [ $# -ne 3 ] ; then
    echo >&3 "usage: $0 <int> <str>"
    exit 1
fi

n="$1"
m="$2"
word="$3"

str= ; i=0
while [ $i -lt $m ] ; do
    str="$str $w"
    i=$((i+1))
done
sh punition1.sh "$n" "$str"
