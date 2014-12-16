#!/bin/bash
#拡散データ格納のディレクトリと作ったグラフのデータを格納するディレクトリを作るスクリプト
#拡散データ格納のディレクトリ(0),グラフのデータを格納するディレクトリ(1),x:追加する枝の数
#使用例　./mkdir_data.sh 0or1 begin end ディレクトリ名

directory=$4
begin=$2
end=$3

if [ $1 = "0" ];then
	cd informspread
	mkdir ${directory}
	cd ${directory}
	i=$begin
	while [ $i -le $end ]
	do
		mkdir ${directory}_${i}_data
		cd ${directory}_${i}_data
		mkdir NC
		mkdir IC
		mkdir DC_desc
		mkdir DC_asc
		mkdir NC_noise
		mkdir IC_noise
		mkdir DC_desc_noise
		mkdir DC_asc_noise
		mkdir IMAGE
		mkdir NOISEIMAGE
		mkdir ./NOISEIMAGE/collision
		mkdir ./NOISEIMAGE/dead
		i=`expr $i + 1`
		cd ..
	done

elif [ $1 = "1" ];then
	mkdir ${directory}
	i=${begin}
	cd ${directory}
	while [ $i -le $end ]
	do
		mkdir ${directory}_${i}_data
		i=`expr $i + 1`
	done
fi