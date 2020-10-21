#!/bin/bash

N=1000
P=2

if [ ! -f "multiply_seq" ]
then
    echo "Nu exista binarul multiply_seq"
    exit
fi

if [ ! -f "multiply2" ]
then
    echo "Nu exista binarul multiply"
    exit
fi

./multiply_seq $N > seq.txt
./multiply2 $N $P > par.txt

diff seq.txt par.txt

rm -rf seq.txt par.txt
