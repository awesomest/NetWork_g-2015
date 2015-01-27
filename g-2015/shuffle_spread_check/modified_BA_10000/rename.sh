#!/bin/bash
#$1:解析したいファイル名(modified_BA_10000)
i=0
while [ $i -le 9 ]
do
j=0
while [ $j -le 99 ]
do
mv ./shuffle_${i}/modified_BA_10000${i}_${j}.dat ./shuffle_${i}/modified_BA_10000_${j}.dat
    j=`expr $j + 1`
done
i=`expr $i + 1`
done