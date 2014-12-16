#!/bin/bash
#$1:使用するディレクトリ名$2:頂点数$3:使うデータの番号
gnuplot << EOF
set terminal postscript eps
set title "dead_${1}_time${3}"
#set out "test.eps"
set xlabel "time"
set ylabel "dead count"
set xrange[0:]
set yrange[0:`expr ${2} + ${2} \* 4 / 10`]
set grid ytics
set out "./informspread/${1}_data/NOISEIMAGE/dead/dead_${1}_time${3}_image.eps"
set style line 1 lt 1 lc rgb "red" lw 3 ps 2 
set style line 2 lt 1 lc rgb "green" lw 3 ps 2
set style line 3 lt 1 lc rgb "blue" lw 3 ps 2
set style line 4 lt 1 lc rgb "orange" lw 3 ps 2
set style line 5 lt 1 lc rgb "magenta" lw 3 ps 2
plot "./informspread/${1}_data/NC_noise/NC_noise_${1}_time${3}.dat" using 1:2 title 'NC' w lp linestyle 1,\
"./informspread/${1}_data/IC_noise/IC_noise_${1}_time${3}.dat" using 1:2 title "IC" w lp linestyle 2,\
"./informspread/${1}_data/DC_asc_noise/DC_asc_noise_${1}_time${3}.dat" using 1:2 title "DC_asc" w lp linestyle 3,\
"./informspread/${1}_data/DC_desc_noise/DC_desc_noise_${1}_time${3}.dat" using 1:2 title "DC_desc" w lp linestyle 4,\
"./informspread/${1}_data/PC_noise/PC_noise_${1}_time${3}.dat" using 1:2 title "PC" w lp linestyle 5
exit
EOF

exit 0