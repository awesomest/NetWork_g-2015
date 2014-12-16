#!/bin/bash
#【直径・半径・平均値の個数を数え、ヒストグラムで可視化するプログラム】2014年01月27日 16時18分24秒
#$1 出力ファイルのテンプレ
#$2 保存するファイルの名前(テンプレ)
#$3 格納しているグラフの数

#実行ファイル名（コンパイル）
countd=count_diameter.cpp

g++ -O2 ${countd}
echo "count start!!"
./a.out ${1}_check.txt count_diameter_${2}.txt count_radius_${2}.txt count_average_${2}.txt ${3}
echo -e "finish\n"

source ${1}_check_main.sh

#all個数をグラフ化(棒グラフ)
echo "diameter ${1%.*} -image(count diameter)-"
gnuplot <<EOF
set term postscript eps enhanced color
set title 'diameter:$d,radius:$r,average:$a'
set xlabel ' '
set ylabel 'total'
set boxwidth 1
set style fill border 2
set style fill solid 0.5
set xtics 1
set ytics 1
set yrange[0:]
set out"image_count_all_${2%.*}.eps"
plot "count_diameter_${2}.txt" using (\$1*4+0):2:xtic(1) title 'diameter' with boxes lt 3  linecolor rgbcolor "red",\
"count_radius_${2}.txt" using (\$1*4+1):2:xtic(1)  title 'radius' with boxes lt 3  linecolor rgbcolor "blue",\
"count_average_${2}.txt" using (\$1*4+2):2:xtic(1) title 'average' with boxes lt 3  linecolor rgbcolor "green"
set terminal x11
exit
EOF
echo "finish"

echo -e "\n"

#直径の個数をグラフ化(棒グラフ)
echo "diameter ${1%.*} -image(count diameter)-"
gnuplot <<EOF
set term postscript eps enhanced color
set title 'diameter distributionss $d'
set xlabel 'diameter'
set ylabel 'total'
set boxwidth 0.8
set style fill border 2
set style fill solid 0.5
set xtics 1
set ytics 1
set yrange[0:]
set out"image_count_diameter_${2%.*}.eps"
plot "count_diameter_${2}.txt" using 1:2 title 'diameter' with boxes lt 3  linecolor rgbcolor "red"
set terminal x11
exit
EOF
echo "finish"

echo -e "\n"
#半径の個数をグラフ化(棒グラフ)
echo "diameter ${1%.*} -image(count radius)-"
gnuplot<<EOF
set term postscript eps enhanced color
set title 'radius distributionss $r'
set xlabel 'radius'
set ylabel 'total'
set boxwidth 0.8
set style fill border 2
set style fill solid 0.5
set xtics 1
set ytics 1
set yrange[0:]
set out"image_count_radius_${2%.*}.eps"
plot "count_radius_${2}.txt" using 1:2 title 'radius' with boxes lt 3  linecolor rgbcolor "blue"
set terminal x11
exit
EOF
echo "finish"

echo -e "\n"

#平均値の個数をグラフ化(棒グラフ)
echo "diameter ${1%.*} -image(count average)-"
gnuplot<<EOF
set term postscript eps enhanced color
set title 'average distributionss $a'
set xlabel 'average'
set ylabel 'total'
set boxwidth 0.8
set style fill border 2
set style fill solid 0.5
set xtics 1
set ytics 1
set yrange[0:]
set out"image_count_average_${2%.*}.eps"
plot "count_average_${2}.txt" using 1:2 title 'average' with boxes lt 3  linecolor rgbcolor "green"
set terminal x11
exit
EOF
echo "finish"

echo -e "\nEND"
