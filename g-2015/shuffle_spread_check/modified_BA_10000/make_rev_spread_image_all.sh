#!/bin/bash
#$1:使用するファイル名 $2:データの始まり $3:データの終わり $4:頂点数 $5:mem/not_mem


if [ ${5} = "mem" ]; then
#for i in {0..9}
#do
	fEnd=${3}
	fCount=${2}
	while [ $fCount -le $fEnd ]
	do
		./make_rev_spread_image.sh 0 ${4} ${fCount} ${5}
#		./make_rev_spread_dead_image.sh ${1} ${4} ${fCount}
#		./make_rev_spread_collision_image.sh ${1} ${4} ${fCount} ${5}
		fCount=`expr $fCount + 1`
	done
#done
elif [ ${5} = "not_mem" ]; then
#for i in {0..9}
#do
	fEnd=${3}
	fCount=${2}
	while [ $fCount -le $fEnd ]
	do
		./make_rev_spread_image.sh 0 ${4} ${fCount} ${5}
#		./make_rev_spread_collision_image.sh ${1} ${4} ${fCount} ${5}
		fCount=`expr $fCount + 1`
	done
fi
#done



