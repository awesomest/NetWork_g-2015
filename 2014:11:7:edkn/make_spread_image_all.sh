#!/bin/bash
#$1:使用するディレクトリ $2:BAのmのbegin $3:BAのmのend $4:データの始まり $5:データの終わり $6:頂点数


mCount=${2}
mEnd=${3}
fEnd=${5}
while [ $mCount -le $mEnd ]
do
	fCount=${4}
	while [ $fCount -le $fEnd ]
	do

		./make_spread_image.sh ${1} ${mCount} ${6} ${fCount}
		./make_spread_dead_image.sh ${1} ${mCount} ${6} ${fCount}
		./make_spread_collision_image.sh ${1} ${mCount} ${6} ${fCount}
		fCount=`expr $fCount + 1`
	done
	mCount=`expr $mCount + 1`
done



