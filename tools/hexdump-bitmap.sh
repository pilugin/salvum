#!/bin/bash

hexdump -e '/32 "%010_ax   |"' -e '32/1 "%_p" "|\n"' $1
