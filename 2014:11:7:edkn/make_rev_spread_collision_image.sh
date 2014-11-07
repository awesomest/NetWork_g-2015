#!/bin/bash
#$1:使用するディレクトリ名$2:頂点数$3:使うデータの番号
gnuplot << EOF
set terminal postscript eps
set title "collision_${1}_time${3}"
#set out "test.eps"
set xlabel "time"
set ylabel "information received"
set xrange[-1:]
set yrange[0:`expr ${2} + ${2} \* 4 / 10`]
set grid ytics
set out "./informspread/${1}_data/NOISEIMAGE/collision/collision_${1}_time${3}_image.eps"
set style line 1 lt 1 lc rgb "red" lw 3 ps 2 
set style line 2 lt 1 lc rgb "green" lw 3 ps 2
set style line 3 lt 1 lc rgb "blue" lw 3 ps 2
set style line 4 lt 1 lc rgb "orange" lw 3 ps 2
plot "./informspread/${1}_data/NC_noise/NC_noise_${1}_time${3}.dat" using 1:3 title 'NC' w p linestyle 1,\
"./informspread/${1}_data/IC_noise/IC_noise_${1}_time${3}.dat" using 1:3 title "IC" w p linestyle 2,\
"./informspread/${1}_data/DC_asc_noise/DC_asc_noise_${1}_time${3}.dat" using 1:3 title "DC_asc" w p linestyle 3,\
"./informspread/${1}_data/DC_desc_noise/DC_desc_noise_${1}_time${3}.dat" using 1:3 title "DC_desc" w p linestyle 4
exit
EOF

exit 0