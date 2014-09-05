#!/bin/sh

#
# Usage: mkbm.sh jpg-file out-file
#

clusterSize=4096
fileSize=$(ls -l $1 | awk '{ print $5 }')
fileClusters=$(( $fileSize / $clusterSize))

if [ $(( $fileClusters * $clusterSize)) -lt $fileSize ]
then
	fileClusters=$(( $fileClusters +1 ))
fi

yes 1 | tr -d '\n' | dd bs=1 count=$fileClusters > $2
