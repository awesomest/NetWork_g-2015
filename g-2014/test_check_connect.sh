#!/bin/bash
#【自己ループ,多重辺,非連結のチェック】2014年01月27日 16時18分24秒 

#$1 出力ファイルのテンプレ
#$2 ファイルの始まり
#$3 ファイルの終わり
#$4 uncorrelate end

#実行ファイル名（コンパイル）
check=check.cpp

g++ -O2 ${check}
#実験プログラムの実行
i=$2
while [ $i -le $3 ];
do
./a.out ${1%.*}_${i}_uc txt 1 ${4}

echo -e "\n"
  i=`expr ${i} + 1`
done

