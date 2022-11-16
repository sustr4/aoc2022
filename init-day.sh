#!/bin/bash

if [ "$1" == "" ]; then
	DAY=`date +%d`
else
	DAY="$1"
fi

mkdir "day${DAY}"
cp init/* "day${DAY}/"
sed -i "s/00/${DAY}/g" "day${DAY}/Makefile"
mv "day${DAY}/day00.c" "day${DAY}/day${DAY}.c"



cd "day${DAY}"
ls
make

echo Now cd into "day${DAY}"

