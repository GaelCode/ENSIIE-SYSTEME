#!/bin/sh

if [ $# -ne 2 ] ; then
    echo "usage: $0 <int> <str>" >&2
    exit 1
fi

n="$1"
word="$2"

i=0
while [ $i -lt "$n" ] ; do
    echo "$word"
    i=$((i+1))
done
