#!/bin/bash

./day07 | datamash -s -g 1 sum 2 |
awk 'BEGIN {sum=0} {
	if($2<100000) sum+=$2}
	END {print sum}'

SIZE=`./day07 | datamash -s -g 1 sum 2 | awk '{print $2}' | sort -n | tail -n 1`
NEEDED=`echo 30000000 - 70000000 + $SIZE | bc`
./day07 | datamash -s -g 1 sum 2 | awk '{print $2}' | sort -n | while read size; do
	if [ $size -ge $NEEDED ]; then
		echo $size
		exit
	fi
done
