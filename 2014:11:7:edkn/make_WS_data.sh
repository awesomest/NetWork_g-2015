#!/bin/bash

#追加する枝がbeginからendまでのBAモデルを作るスクリプト
#begin end は偶数
#入力例 ./make_graph_data.sh サイクルのテンプレ名　WSのテンプレ名　グラフの頂点数 確率　作成ファイル数 begin end

i=$6
g++ make_WS.cpp -o make_WS
N=$3
filecount=$5
while [ $i -le $7 ]
do
	CYCLE_directory="${1}/${1}_${i}_data"
	CYCLE_filename="${1}_${i}"
	WS_directory="${2}/${2}_${i}_data"
	WS_filename="${2}_${i}"
	#echo $directory
	#echo $filename
	./make_WS ${N} ${i} ${4} ${filecount} ${CYCLE_directory} ${CYCLE_filename} ${WS_directory} ${WS_filename}
	i=`expr $i + 2`
done

exit 0