#!/bin/bash
#$1:ディレクトリ名 $2:mの始まり $3:mの終わり

begin=${2}
end=$3
while [ $begin -le $end ]
do
	mkdir ./informspread/${1}/${1}_${begin}_data/IMAGE
	mkdir ./informspread/${1}/${1}_${begin}_data/NOISEIMAGE
	mkdir ./informspread/${1}/${1}_${begin}_data/NOISEIMAGE/dead
	mkdir ./informspread/${1}/${1}_${begin}_data/NOISEIMAGE/collision
	begin=`expr $begin + 1`
done