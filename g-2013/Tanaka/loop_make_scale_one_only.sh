#!/bin/bash
#【指定した回数、スケールフリーグラフを制作するプログラムを実行するプログラム】2013.01/19,00:52 Yuho.Tanaka
#$1 グラフのファイル名（BA）
#$2 最大頂点数
#$3 作成ファイル数の始まり
#$4 作成ファイル数の終わり

#実行ファイル名（コンパイル）
scale=make_scale__one_only.cpp

i=$3
while [ $i -le $4 ];
do
    echo "scale[$i]"
    g++ -O2 ${scale}
    ./a.out ${1%.*}_${i}.${1##*.} ${2}
    i=`expr ${i} + 1`
done

echo -e "\nEND"