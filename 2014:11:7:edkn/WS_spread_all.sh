#!/bin/bash
#ファイル名のテンプレ、ディレクトリ名のテンプレを渡して mがbeginからendまでの情報伝播を行いデータを作成
#入力例 ./spread.sh ファイル名　 頂点数　使うネットワークの数　一つのネットワークに対する実験回数　存在しているデータの数　begin end
g++ InformSpread.cpp -o is
N=$2
usedata=$3
trial=$4
exist=$5

begin=${6}
end=${7}
for i in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0
do
	j=$begin
	while [ $j -le $end ]
	do
		filename="${1}_${i}_${j}"
		directory="${1}_${i}/${1}_${i}_${j}_data"
		#echo $filename
		./is ${filename} ${1}/${directory} ${N} ${usedata} ${trial} ${exist}
		j=`expr $j + 2`
	done
done
exit 0