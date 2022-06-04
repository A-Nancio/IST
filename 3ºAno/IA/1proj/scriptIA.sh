#!/bin/bash

inputdir=$1

for file in ${inputdir}/*.rr
do
    echo -e "${file}"
    timelimit -t30 python3 ricochet_robots.py ${file}
done