#!/bin/bash
#$1:使用するディレクトリ $2:データの始まり $3:データの終わり $4 頂点数

fCount=${2}
fEnd=${3}
while [ $fCount -le $fEnd ]
do
	./degree_image.sh ${1} ${4} ${fCount}
	fCount=`expr $fCount + 1`
done