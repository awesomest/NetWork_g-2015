#!/bin/bash
#$1:解析したいファイル名(modified_BA_10000)
g++ -Wall InformSpread_not_mem.cpp -o spread_not_mem
i=0
while [ $i -le 9 ]
do
./spread_not_mem ${1} shuffle_${i} 10000 100 100 100 100
echo finish_granph_${i}
    i=`expr $i + 1`
done