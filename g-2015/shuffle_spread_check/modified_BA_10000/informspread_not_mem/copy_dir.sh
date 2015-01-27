#!/bin/bash

mkdir ./shuffle_0
mkdir ./shuffle_0/IC
mkdir ./shuffle_0/PC
mkdir ./shuffle_0/NC
mkdir ./shuffle_0/IMAGE
i=1
while [ $i -le 99 ]
do
cp -a ./shuffle_0 ./shuffle_${i}
i=`expr $i + 1`
done