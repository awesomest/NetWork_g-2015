#!/bin/bash

#追加する枝がbeginからendまでのBAモデルを作るスクリプト
#入力例 ./make_graph_data.sh ディレクトリ名　ファイル名のテンプレ 　グラフの頂点数 確率　作成ファイル数 begin end

i=$5
g++ make_WS.cpp -o make_WS
N=$3
filecount=$4
while [ $i -le $6 ]
do
	directory="${1}/${1}_${i}_data"
	filename="${1}_${i}"
	#echo $directory
	#echo $filename
	./make_WS ${i} ${N} ${i} ${filecount} ${directory} ${filename}
	i=`expr $i + 2`
done

exit 0