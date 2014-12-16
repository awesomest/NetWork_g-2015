#!/bin/bash
#$1:使用するディレクトリ名　$2:頂点数 $3:使うデータの番号
gnuplot << EOF
set terminal postscript eps
set title "degree_distribution_${1}_${3}"
set xlabel "degree"
set ylabel "count"
set xrange [0:]
set yrange [0:]
set grid ytics
set out "./degree/${1}_data/image/image_${1}_${3}.eps"
set style line 1 lt 1 lc rgb "red" lw 3
plot "./degree/${1}_data/degree_data/degree_${1}_${3}.dat" using 1:2 title "degree" w p linestyle 1
exit
EOF

exit 0