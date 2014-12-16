#!/bin/bash
#ファイル名のテンプレ mがbeginからendまでの情報伝播を行いデータを作成
#入力例 ./spread.sh ファイル名　 頂点数　使うネットワークの数　一つのネットワークに対する実験回数　存在しているデータの数　begin end
g++ InformSpread.cpp -o is
N=$2
usedata=$3
trial=$4
exist=$5
end=$7
i=$6
while [ $i -le $end ]
do
	filename="$1_$i"
	directory="$1_$i""_data"
	./is ${filename} ${1}/${directory} ${N} ${usedata} ${trial} ${exist}
	i=`expr $i + 1`
done

exit