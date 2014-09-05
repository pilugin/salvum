#!/bin/sh

#
# Usage: shuf.sh jpg-file bm-file out-file
#

clusterSize=4096
fileSize=$(ls -l $1 | awk '{ print $5 }')
fileClusters=$(( $fileSize / $clusterSize))

if [ $(( $fileClusters * $clusterSize)) -lt $fileSize ]
then
	fileClusters=$(( $fileClusters +1 ))
fi

# copy first 20 clusters without shuffle
noShuffleClusters=20

dd if=$1 of=$3    bs=$clusterSize count=$noShuffleClusters
dd if=$2 of=$3.bm bs=1            count=$noShuffleClusters
seq 0 $(($noShuffleClusters-1)) | xargs printf '%08X\n' > $3.shufflemap

map=($(seq $noShuffleClusters $(($fileClusters -1 -1)) |shuf))
echo ${#map[*]}
for i in ${map[*]}
do
    printf '%08X\n' $i >> $3.shufflemap

    dd if=$1 bs=$clusterSize count=1 skip=$i >> $3
    dd if=$2 bs=1            count=1 skip=$i >> $3.bm

done
   
# copy last cluster at END as it can be less then clusterSize
dd if=$1 bs=$clusterSize skip=$(($fileClusters -1)) >> $3
dd if=$2 bs=1            skip=$(($fileClusters -1)) >> $3.bm

printf '%08X\n' $(($fileClusters -1)) >> $3.shufflemap
