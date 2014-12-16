#!/bin/bash
#【修正BAでネットワークを指定した個数作成し,指定した個数組み替えるプログラム】
#$1 出力ファイルのテンプレ
#$2 最大頂点数
#$3 作成ファイル数の始まり
#$4 作成ファイル数の終わり
#$5 uncorrelate end

#実行ファイル名（コンパイル）
make=make_shuuseiBA.cpp
uncorrelate=graph_shuffle.cpp

#スケールフリーグラフを作成
echo "scale[$i] -make scale-"
g++ -O2 ${make}
./a.out ${1%.*} txt ${2} ${3} ${4}

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
