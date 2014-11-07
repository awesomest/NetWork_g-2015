#!/bin/bash
#WSモデルのディレクトリ作成
#拡散データ格納のディレクトリと作ったグラフのデータを格納するディレクトリを作るスクリプト
#拡散データ格納のディレクトリ(0),グラフのデータを格納するディレクトリ(1) begin endは偶数
#使用例　./mkdir_WS_data.sh 0or1 begin end ディレクトリ名

directory=$4
begin=$2
end=$3

if [ $1 = "0" ];then
	cd informspread
	mkdir ${directory}
	cd ${directory}
	for i in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0
	do
		mkdir ${directory}_${i}
		cd ${directory}_${i}
		mkdir ${directory}_${i}_${begin}_data
		cd ${directory}_${i}_${begin}_data
		mkdir NC
		mkdir IC
		mkdir DC_desc
		mkdir DC_asc
		mkdir NC_noise
		mkdir IC_noise
		mkdir DC_desc_noise
		mkdir DC_asc_noise
		j=`expr $begin + 2`
		while [ $j -le $end ]
		do
			#echo "../${directory}_${begin}_data"
			cp -r ../${directory}_${i}_${begin}_data ../${directory}_${i}_${j}_data
			j=`expr $j + 2`
		done
		cd ../../
	done	
elif [ $1 = "1" ];then
	mkdir ${directory}
	cd ${directory}
	for i in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0
	do
		mkdir ${directory}_${i}
		j=${begin}
		while [ $j -le $end ]
		do
			mkdir ./${directory}_${i}/${directory}_${i}_${j}_data
			j=`expr $j + 2`
		done
	done
fi