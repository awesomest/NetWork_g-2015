#!/bin/bash
#$1:使用するディレクトリ名$2:BAのm$3:頂点数$4:使うデータの番号
gnuplot << EOF
set terminal postscript eps
set title "dead_${1}_${2}_time${4}"
#set out "test.eps"
set xlabel "time"
set ylabel "information received"
set xrange[0:]
set yrange[0:`expr ${3} + ${3} \* 4 / 10`]
set grid ytics
set out "./informspread/${1}/${1}_${2}_data/NOISEIMAGE/dead/dead_${1}_${2}_time${4}_image.eps"
set style line 1 lt 1 lc rgb "red" lw 3 ps 2 
set style line 2 lt 1 lc rgb "green" lw 3 ps 2
set style line 3 lt 1 lc rgb "blue" lw 3 ps 2
set style line 4 lt 1 lc rgb "orange" lw 3 ps 2
plot "./informspread/${1}/${1}_${2}_data/NC_noise/NC_noise_${1}_${2}_time${4}.dat" using 1:2 title 'NC' w lp linestyle 1,\
"./informspread/${1}/${1}_${2}_data/IC_noise/IC_noise_${1}_${2}_time${4}.dat" using 1:2 title "IC" w lp linestyle 2,\
"./informspread/${1}/${1}_${2}_data/DC_asc_noise/DC_asc_noise_${1}_${2}_time${4}.dat" using 1:2 title "DC_asc" w lp linestyle 3,\
"./informspread/${1}/${1}_${2}_data/DC_desc_noise/DC_desc_noise_${1}_${2}_time${4}.dat" using 1:2 title "DC_desc" w lp linestyle 4
exit
EOF

exit 0