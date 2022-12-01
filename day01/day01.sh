#!/bin/bash

./day01 | sort -n | tail -n 1

./day01 | sort -n | tail -n 3 | awk '{sum+=$1} END {print sum}'
