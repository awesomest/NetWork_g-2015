#!/bin/bash

#追加する枝がbeginからendまでのBAモデルを作るスクリプト
#begin end は偶数
#入力例 ./make_graph_data.sh WSのテンプレ名　グラフの頂点数 　作成ファイル数 begin end


g++ make_WS.cpp -o make_WS
N=$2
filecount=$3
begin=$4
end=$5
for i in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0
do
	j=$begin
	while [ $j -le $end ]
	do
		WS_directory="${1}/${1}_${i}/${1}_${i}_${j}_data"
		#echo ${WS_directory}
		WS_filename="${1}_${i}_${j}"
		./make_WS ${N} ${j} ${i} ${filecount}  ${WS_directory} ${WS_filename}
		j=`expr ${j} + 2`
	done
done
exit 0