#!/bin/bash
#【実験プログラムの実行用】2014年01月27日 16時18分24秒
#$1 出力ファイルのテンプレ
#$2 最大頂点数
#$3 ファイルの始まり
#$4 ファイルの終わり
#$5 uncorrelate start
#$6 uncorrelate end

#実行ファイル名（コンパイル）
make=test_image_scale.sh
histogram=test_image_histogram.sh
check=test_check_connect.sh
check2=check_diameter_for_loop.sh


#実験プログラムの実行
chmod 755 ${make}
time ./${make} ${1%.*} ${2} ${3} ${4} ${6}
echo -e "(test_image_ba.sh)\n"

chmod 755 ${check2}
time ./${check2} ${1%.*} ${3} ${4}
echo "(check_diameter_for_loop.sh)"

chmod 755 ${histogram}
time ./${histogram} ${1%.*} ${3} ${4} ${6}
echo "(test_image_histogram.sh)"

chmod 755 ${check}
time ./${check} ${1%.*} ${3} ${4} ${6} > ${1%.*}_check_graph.txt
echo "(test_check_connect.sh)"

