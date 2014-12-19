#!/bin/bash
#$1:ファイル名のテンプレ$2作るデータの数
name=$1
makedata=$2
#cd uncorrelated
mkdir "./uncorrelated/${name}_data"
i=0
while [ $i -le $makedata ]
do
	mkdir "./uncorrelated/${name}_data/${name}_${i}"
	mkdir "./uncorrelated/${name}_data/${name}_${i}/shuffle"
	mkdir "./uncorrelated/${name}_data/${name}_${i}/distance"
	mkdir "./uncorrelated/${name}_data/${name}_${i}/degree"
	i=`expr $i + 1`
done

