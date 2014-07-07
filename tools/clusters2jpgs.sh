#!/bin/sh

ls $1/*clusters | while read i
do
    cat $i
    `dirname $0`/extract-clusters.sh $2 < $i > `basename $i .clusters`.jpg
done