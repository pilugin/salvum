#!/bin/sh

#
# Usage: mkbm.sh jpg-file out-file
#

clusterSize=4096
fileClusters=$(( $(ls -l $1 | awk '{ print $5 }') / $clusterSize +1))
yes 1 | tr -d '\n' | dd bs=1 count=$fileClusters > $2
