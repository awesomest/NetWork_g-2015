#!/bin/bash
#【直径・半径・平均値を調べ、各々の個数を数え、ヒストグラムで可視化するプログラム】2014年01月27日 16時18分24秒

#$1 出力ファイルのテンプレ
#$2 ファイルの始まり
#$3 ファイルの終わり
#$4 uncorrelate end

#実行ファイル名（コンパイル）
check=check_diameter_for_loop_r.sh
image=test_image_bougraph.sh
#実験プログラムの実行
chmod 755 ${check}
./${check} ${1%.*} ${2} ${3} ${4}

chmod 755 ${image}
i=$2
while [ $i -le $3 ];
do
./${image} ${1%.*}_${i} ${1%.*}_${i} ${4}

    i=`expr ${i} + 1`
done

