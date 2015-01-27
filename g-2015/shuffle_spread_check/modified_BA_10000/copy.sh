#!/bin/bash
i=0
while [ $i -le 99 ]
do
cp -a ../uncorrelated/modified_BA_10000_data/modified_BA_10000_${i}/shuffle ./shuffle_${i}
i=`expr $i + 1`
done