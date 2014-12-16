#!/bin/bash
#$1:使用するディレクトリ名$2:頂点数$3:使うデータの番号$4:mem/not_mem
if [ ${4} = "mem" ]; then
gnuplot << EOF
set terminal postscript eps
set title "${1}_time${3}_mem"
#set out "test.eps"
set xlabel "time"
set ylabel "information received"
set xrange[0:]
set yrange[0:`expr ${2} + ${2} \* 4 / 10`]
set grid ytics
set out "./informspread/${1}_data/IMAGE/${1}_time${3}_image.eps"
set style line 1 lt 1 lc rgb "red" lw 3 ps 2 
set style line 2 lt 1 lc rgb "green" lw 3 ps 2
set style line 3 lt 1 lc rgb "blue" lw 3 ps 2
set style line 4 lt 1 lc rgb "orange" lw 3 ps 2
set style line 5 lt 1 lc rgb "magenta" lw 3 ps 2
plot "./informspread/${1}_data/NC/NC_${1}_time${3}.dat" using 1:2 title 'NC' w lp linestyle 1,\
"./informspread/${1}_data/IC/IC_${1}_time${3}.dat" using 1:2 title "IC" w lp linestyle 2,\
"./informspread/${1}_data/DC_asc/DC_asc_${1}_time${3}.dat" using 1:2 title "DC_asc" w lp linestyle 3,\
"./informspread/${1}_data/DC_desc/DC_desc_${1}_time${3}.dat" using 1:2 title "DC_desc" w lp linestyle 4,\
"./informspread/${1}_data/PC/PC_${1}_time${3}.dat" using 1:2 title "PC" w lp linestyle 5
exit
EOF
elif [ ${4} = "not_mem" ];then
gnuplot << EOF
set terminal postscript eps
set title "${1}_time${3}_not_mem"
#set out "test.eps"
set xlabel "time"
set ylabel "information received"
set yrange[0:`expr ${2} + ${2} \* 4 / 10`]
#set	logscale x
set grid ytics
set out "./informspread_not_mem/${1}_data/IMAGE/${1}_time${3}_image.eps"
set style line 1 lt 1 lc rgb "red" lw 3 ps 2 
set style line 2 lt 1 lc rgb "green" lw 3 ps 2
set style line 5 lt 1 lc rgb "magenta" lw 3 ps 2
plot "./informspread_not_mem/${1}_data/NC/NC_${1}_time${3}.dat" using 1:2 title 'NC' w l linestyle 1,\
"./informspread_not_mem/${1}_data/IC/IC_${1}_time${3}.dat" using 1:2 title "IC" w l linestyle 2,\
"./informspread_not_mem/${1}_data/PC/PC_${1}_time${3}.dat" using 1:2 title "PC" w l linestyle 5
exit
EOF
fi
exit 0