#!/bin/bash

while read A B 
do

 dd if=$1 bs=4096 count=$((0x$B - 0x$A +1)) skip=$((0x$A)) 2>/dev/null

done

dd if=/dev/zero bs=4096 count=100 2>/dev/null

	
