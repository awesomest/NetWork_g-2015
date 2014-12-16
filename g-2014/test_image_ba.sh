#!/bin/bash
#【BAでネットワークを指定した個数作成し,指定した個数組み替えるプログラム】
#$1 出力ファイルのテンプレ
#$2 追加する頂点数
#$3 作成ファイル数の始まり
#$4 作成ファイル数の終わり
#$5 uncorrelate end
#$6 完全グラフの頂点数

#実行ファイル名（コンパイル）
make=make_BA.cpp
uncorrelate=graph_shuffle.cpp

#スケールフリーグラフを作成
echo "scale[$i] -make scale-"
g++ -O2 ${make}
./a.out ${6} ${2} ${3} ${4} ${1%.*} txt

#uncorrelate作成
g++ -O2 ${uncorrelate}
i=$3
while [ $i -le $4 ];
do
    echo "uncorrelate[${i}] -make uncorrelate-"
    ./a.out ${1%.*}_${i}.txt ${1%.*}_${i}_uc txt 1 ${5}
    i=`expr ${i} + 1`
done

echo -e "\nEND"
