#!/bin/bash
#【スケールフリーのグラフをを指定した個数作成し、その次数、グラフの直径、半径、平均値を測定（forループ）し、次数分布を測定⇒図へ保存（nomal）するプログラム】2013.01/19,00:30 Yuho.Tanaka
#$1 グラフのファイル名（BA）
#$2 最大頂点数
#$3 作成ファイル数の始まり
#$4 作成ファイル数の終わり

#実行ファイル名（コンパイル）
#make=make_scale__for_loop.cpp
count=count_degree.cpp
diametr=check_diametr__for_loop.cpp
plotN=plot_nomal.cpp
plotR=plot_ranking.cpp
plotRS=plot_ranking_zeroskip.cpp


#スケールフリーグラフを作成
#echo "scale[$i] -make scale-"
#g++ -O2 ${make}
#./a.out ${1%.*} ${1##*.} ${2} ${3} ${4}

#作成したグラフの次数を測定
g++ -O2 ${count}
i=$3
while [ $i -le $4 ];
do
    echo "scale[$i] -degree-"
    ./a.out ${1%.*}_${i}.${1##*.} degree_${1%.*}_${i}.${1##*.}
    i=`expr ${i} + 1`
done

#作成したグラフの直径、半径、平均値を測定（forループ）
g++ -O2 ${diametr}
i=$3
while [ $i -le $4 ];
do
    echo -e "\nscale[$i] -diametr-"
    ./a.out ${1%.*}_${i}.${1##*.} diametr__for_${1%.*}.${1##*.} ${i}
    i=`expr ${i} + 1`
done

echo -e "\n"

#次数分布を測定
g++ -O2 ${plotN}
i=$3
while [ $i -le $4 ];
do
    echo "scale[$i] -plot nomal-"
    ./a.out ${1%.*}_${i}.${1##*.} plot_nomal_${1%.*}_${i}.${1##*.}
    i=`expr ${i} + 1`
done

echo -e "\n"

#次数分布を測定(累計順位)
g++ -O2 ${plotR}
i=$3
while [ $i -le $4 ];
do
    echo "scale[$i] -plot ranking-"
    ./a.out ${1%.*}_${i}.${1##*.} plot_ranking_${1%.*}_${i}.${1##*.}
    i=`expr ${i} + 1`
done

echo -e "\n"

#次数分布を測定(累計順位（0skip）)
g++ -O2 ${plotRS}
i=$3
while [ $i -le $4 ];
do
    echo "scale[$i] -plot ranking 0skip-"
    ./a.out ${1%.*}_${i}.${1##*.} plot_ranking_zeroskip_${1%.*}_${i}.${1##*.}
    i=`expr ${i} + 1`
done

echo -e "\n"

#図へ保存
i=$3
while [ $i -le $4 ];
do
echo "scale[$i] -image(plot nomal)-"
gnuplot <<EOF
set terminal postscript eps
set title 'degree distributions'
set xlabel 'k'
set ylabel 'p(k)'
set logscale
plot "plot_nomal_${1%.*}_${i}.${1##*.}" using 1:3 title 'degree' with points pt 7 ps 1
set out "image_nomal_${1%.*}_${i}.eps"
replot
EOF
echo "finish"
i=`expr ${i} + 1`
done

echo -e "\n"

#図へ保存(累計順位)
i=$3
while [ $i -le $4 ];
do
echo "scale[$i] -image(plot ranking)-"
gnuplot <<EOF
set terminal postscript eps
set title 'degree distributions -ranking-'
set xlabel 'k'
set ylabel 'total distributions'
set logscale
plot "plot_ranking_${1%.*}_${i}.${1##*.}" using 1:3 title 'degree' with points pt 7 ps 1
set out "image_ranking_${1%.*}_${i}.eps"
replot
EOF
echo "finish"
i=`expr ${i} + 1`
done

echo -e "\n"

#図へ保存(累計順位（0skip）)
i=$3
while [ $i -le $4 ];
do
echo "scale[$i] -image(plot ranking 0skip)-"
gnuplot <<EOF
set terminal postscript eps
set title 'degree distributions -ranking 0skip-'
set xlabel 'k'
set ylabel 'total distributions'
set logscale
plot "plot_ranking_zeroskip_${1%.*}_${i}.${1##*.}" using 1:3 title 'degree' with points pt 7 ps 1
set out "image_ranking_zeroskip_${1%.*}_${i}.eps"
replot
EOF
echo "finish"
i=`expr ${i} + 1`
done

echo -e "\nEND"
