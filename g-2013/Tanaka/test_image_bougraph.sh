#!/bin/bash
#【直径・半径・平均値の個数を数え、グラフ化するプログラム】2013.01/30,14:33 Yuho.Tanaka
#$1 読み取るファイルの名前
#$2 保存するファイルの名前
#$3 格納しているグラフの数

#実行ファイル名（コンパイル）
countd=count_diametr.cpp

g++ -O2 ${countd}
echo "count start!!"
./a.out ${1} count_diametr_${2} count_radius_${2} count_average_${2} ${3}
echo -e "finish\n"

#直径の個数をグラフ化(棒グラフ)
echo "diametr ${1%.*} -image(count diametr)-"
gnuplot<<EOF
set term postscript eps enhanced color
plot "count_diametr_${2}" using 1:2 title 'diametr' with boxes lt 3
set title 'diameter distributionss'
set xlabel 'diameter'
set ylabel 'total'
set boxwidth 0.8
set style fill border 2
set style fill solid 0.5
set xtics 1
set out"image_count_diametr_${2%.*}.eps"
replot
exit
EOF
echo "finish"

echo -e "\n"

#半径の個数をグラフ化(棒グラフ)
echo "diametr ${1%.*} -image(count radius)-"
gnuplot<<EOF
set term postscript eps enhanced color
plot "count_radius_${2}" using 1:2 title 'radius' with boxes lt 3
set title 'radius distributionss'
set xlabel 'radius'
set ylabel 'total'
set boxwidth 0.8
set style fill border 2
set style fill solid 0.5
set xtics 1
set out"image_count_radius_${2%.*}.eps"
replot
exit
EOF
echo "finish"

echo -e "\n"

#平均値の個数をグラフ化(棒グラフ)
echo "diametr ${1%.*} -image(count average)-"
gnuplot<<EOF
set term postscript eps enhanced color
plot "count_average_${2}" using 1:2 title 'average' with boxes lt 3
set title 'average distributionss'
set xlabel 'average'
set ylabel 'total'
set boxwidth 0.8
set style fill border 2
set style fill solid 0.5
set xtics 1
set out"image_count_average_${2%.*}.eps"
replot
exit
EOF
echo "finish"

echo -e "\nEND"