#!/bin/bash
#【スケールフリーのグラフをを指定した個数作成し、その次数、グラフの直径、半径、平均値を測定（forループ）し、次数分布を測定⇒図へ保存（nomal）するプログラム】2013.01/19,06:16 Yuho.Tanaka
#$1 グラフのファイル名（BA）
#$2 作成ファイル数

#実行ファイル名（コンパイル）
plotDC=plot_diametr_chenge.cpp

#各グラフの直径・半径・平均値・平均値をプロットしやすいよう並び替え
g++ -O2 ${plotDC}
echo "diametr ${1%.*} -chenge-"
./a.out ${1} plot_${1} ${2}

echo -e "\n"

#図へ保存(各グラフの直径)
echo "diametr ${1%.*} -image(plot diametr)-"
gnuplot <<EOF
set terminal postscript eps
set title 'diameter distributions'
set xlabel 'scale'
set ylabel 'diametr'
set yrange[1:]
plot "plot_${1}" using 1:2 title 'diametr' with points pt 7 ps 1
set out "image_diametr_${1%.*}.eps"
replot
EOF
echo "finish"

echo -e "\n"

#図へ保存(各グラフの半径)
echo "diametr ${1%.*} -image(plot radius)-"
gnuplot <<EOF
set terminal postscript eps
set title 'radius distributions'
set xlabel 'scale'
set ylabel 'radius'
set yrange[1:]
plot "plot_${1}" using 1:3 title 'radius' with points pt 7 ps 1
set out "image_radius_${1%.*}.eps"
replot
EOF
echo "finish"

echo -e "\n"

#図へ保存(各グラフの平均値)
echo "diametr ${1%.*} -image(plot average)-"
gnuplot <<EOF
set terminal postscript eps
set title 'average distributions'
set xlabel 'scale'
set ylabel 'average'
set yrange[1:]
plot "plot_${1}" using 1:4 title 'average' with points pt 7 ps 1
set out "image_average_${1%.*}.eps"
replot
EOF
echo "finish"

echo -e "\nEND"
