#!/bin/sh

#
# Usage: ./mesh.sh jpg-file junk-file out-file
# 

clusterSize=4096
clusterOffset=0
step=100
fileClusters=$(( $(ls -l $1 | awk '{ print $5 }') / $clusterSize ))

truncate --size 0 $3
truncate --size 0 $3.bm

while [ $clusterOffset -lt $fileClusters ] 
do
    dd if=$1 bs=$clusterSize skip=$clusterOffset count=$step    >> $3
    dd if=$2 bs=$clusterSize                     count=$step    >> $3
    dd if=/dev/zero bs=$clusterSize              count=$step    >> $3
    
    yes 1 | tr -d '\n' | dd bs=1 count=$(($step*2))             >> $3.bm
    yes 0 | tr -d '\n' | dd bs=1 count=$step                    >> $3.bm
    
    clusterOffset=$(($clusterOffset + $step))
done
