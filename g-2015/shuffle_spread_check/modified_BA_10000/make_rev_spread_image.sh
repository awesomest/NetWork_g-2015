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
set out "./informspread/shuffle_${1}/IMAGE/modified_BA_10000_time${3}_image.eps"
set style line 1 lt 1 lc rgb "red" lw 3 ps 2 
set style line 2 lt 1 lc rgb "green" lw 3 ps 2
set style line 3 lt 1 lc rgb "blue" lw 3 ps 2
set style line 4 lt 1 lc rgb "orange" lw 3 ps 2
set style line 5 lt 1 lc rgb "magenta" lw 3 ps 2
plot "./informspread/shuffle_${1}/NC/NC_modified_BA_10000_time${3}.dat" using 1:2 title 'NC' w lp linestyle 1,\
"./informspread/shuffle_${1}/IC/IC_modified_BA_10000_time${3}.dat" using 1:2 title "IC" w lp linestyle 2,\
"./informspread/shuffle_${1}/DC_asc/DC_asc_modified_BA_10000_time${3}.dat" using 1:2 title "DC_asc" w lp linestyle 3,\
"./informspread/shuffle_${1}/DC_desc/DC_desc_modified_BA_10000_time${3}.dat" using 1:2 title "DC_desc" w lp linestyle 4,\
"./informspread/shuffle_${1}/PC/PC_modified_BA_10000_time${3}.dat" using 1:2 title "PC" w lp linestyle 5
exit
EOF
elif [ ${4} = "not_mem" ];then
gnuplot << EOF
set terminal postscript eps color
set title "modified BA10000 time${3} not mem"
#set out "test.eps"
set xlabel "time"
set ylabel "information received"
set yrange[0:`expr ${2} + ${2} \* 4 / 10`]
#set	logscale x
set grid ytics
set out "./informspread_not_mem/shuffle_${1}/IMAGE/modified_BA_10000_time${3}_image.eps"
set style line 1 lt 1 lc rgb "red" lw 3 ps 2 
set style line 2 lt 1 lc rgb "green" lw 3 ps 2
set style line 5 lt 1 lc rgb "magenta" lw 3 ps 2
plot "./informspread_not_mem/shuffle_${1}/NC/NC_modified_BA_10000_time${3}.dat" using 1:2 title 'NC' w l linestyle 1,\
"./informspread_not_mem/shuffle_${1}/IC/IC_modified_BA_10000_time${3}.dat" using 1:2 title "IC" w l linestyle 2,\
"./informspread_not_mem/shuffle_${1}/PC/PC_modified_BA_10000_time${3}.dat" using 1:2 title "PC" w l linestyle 5
exit
EOF
fi
exit 0