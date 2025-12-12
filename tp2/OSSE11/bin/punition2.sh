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

i=0
while [ $i -lt "$n" ] ; do
    y=0
    while [ $y -lt "$m" ] ; do
        echo -n "$word "
        y=$((y+1))
    done
    
    i=$((i+1))
    echo ""
done

