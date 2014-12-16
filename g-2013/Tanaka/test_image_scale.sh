#!/bin/bash
#【スケールフリーのグラフをを指定した個数作成し、その次数調べ、グラフを可視化するプログラム】2013.01/18,12:07 Yuho.Tanaka
#$1 グラフのファイル名（BA）
#$2 最大頂点数
#$3 作成ファイル数の始まり
#$4 作成ファイル数の終わり

#実行ファイル名（コンパイル）
#make=make_scale__for_loop.cpp
count=count_degree2.cpp
graph=graph_degree.rb

#スケールフリーグラフを作成
#echo "scale[$i] -make scale-"
#g++ -O2 ${make}
#./a.out ${1%.*} ${1##*.} ${2} ${3} ${4}

#作成したグラフの次数を測定
g++ -O2 ${count}
i=$3
while [ $i -le $4 ];
do
    echo "scale[$i] -degree-"
    ./a.out ${1%.*}_${i}.${1##*.} degree_${1%.*}_${i}.${1##*.} degree_frontskip_${1%.*}_${i}.csv
    i=`expr ${i} + 1`
done

#作成したグラフを可視化
i=$3
while [ $i -le $4 ];
do
    echo "scale[$i] -scale graph-"
    ruby ${graph} degree_frontskip_${1%.*}_${i}.csv image_scale_${1%.*}_${i} png
    i=`expr ${i} + 1`
done

echo -e "\nEND"
