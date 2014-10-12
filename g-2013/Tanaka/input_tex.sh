#!/bin/bash
#【texに大量に画像を挿入するプログラム】2013.02/04,14:26 Yuho.Tanaka

#--入力----------------------------------------

echo "保存するファイル名を入力してください"
read make_name

echo "生成したスケールフリー関連の画像は各３００枚まであります"
echo "挿入する画像の開始番号を入力してください"
read start_num

echo "画像をいくつ飛ばしで挿入するのか入力してください"
read cd_num

echo "挿入する画像の終了番号を入力してください"
read end_num

echo "挿入時の画像サイズを入力してください"
read view_size

echo "挿入時の画像番号を入力してください（2013.02/04の時では10枚挿入中）"
read input_num

#--関数1
#$1:保存するファイル名
#$2:挿入する画像名
#$3:挿入する画像名の番号
#$4:挿入時の画像サイズ
#$5:累計画像番号
#$6:挿入時の画像名

input_view(){
echo "\begin{center}">>${1}
echo "\includegraphics[width=${4}cm]{${2%.*}_${3}.${2##*.}}\\\\">>${1}
echo "\figurename{${5}: ${6}-${3}-}">>${1}
echo "\end{center}">>${1}
echo >>${1}
}

#--関数
#$1:保存するファイル名
#$2:挿入する画像名
#$3:挿入時の画像サイズ
#$4:累計画像番号
#$5:挿入時の画像名

input_view2(){
echo "\begin{center}">>${1}
echo "\includegraphics[width=${3}cm]{${2%.*}.${2##*.}}\\\\">>${1}
echo "\figurename{${4}: ${5}}">>${1}
echo "\end{center}">>${1}
echo >>${1}
}

#--ファイルを作成

touch ${make_name}

#--節番号の記入

echo "\section{実験結果}">>${make_name}
echo >>${make_name}

#--修正版BAモデルのネットワークの画像の挿入

echo "ネットワークを可視化"
echo "\subsection{修正版BAモデルのネットワーク}">>${make_name}
echo >>${make_name}

i=1
while [ `expr ${start_num} + '(' ${i} - 1 ')' '*' ${cd_num}` -le ${end_num} ]
do
input_view ${make_name} "image_scale_1000.png" `expr ${start_num} + '(' ${i} - 1 ')' '*' ${cd_num}` ${view_size} ${input_num} "頂点数1000のネットワーク"
i=`expr ${i} + 1`
input_num=`expr ${input_num} + 1`
done

echo "END"

#--直径,半径,平均値の度数分布グラフを描画

echo "直径,半径,平均値の度数分布グラフ"
echo "\subsection{直径,半径,平均値の度数分布グラフ}">>${make_name}
echo >>${make_name}

for vertex in 1000 10000 100000
do
echo "\subsubsection{頂点数${vertex}}">>${make_name}
echo >>${make_name}
for s in diametr radius average
do
input_view2 ${make_name} "image_count_${s}_${vertex}.eps" ${view_size} ${input_num} "${s}の度数分布グラフ"
input_num=`expr ${input_num} + 1`
done
done

echo "END"

#--次数分布グラフ（次数分布の両対数を描写）

echo "次数分布グラフ（次数分布の両対数を描写）"
echo "\subsection{次数分布グラフ（次数分布の両対数を描写）}">>${make_name}
echo >>${make_name}

for vertex in 1000 10000 100000
do
i=1
while [ `expr ${start_num} + '(' ${i} - 1 ')' '*' ${cd_num}` -le ${end_num} ]
do
input_view ${make_name} "image_nomal_${vertex}.eps" `expr ${start_num} + '(' ${i} - 1 ')' '*' ${cd_num}` ${view_size} ${input_num} "頂点数${vertex}の次数分布グラフ"
i=`expr ${i} + 1`
input_num=`expr ${input_num} + 1`
done
done

echo "END"

#--次数分布グラフ（元の次数分布の累積分布の両対数で描画）

echo "次数分布グラフ（元の次数分布の累積分布の両対数で描画)"
echo "\subsection{次数分布グラフ（元の次数分布の累積分布の両対数で描画）}">>${make_name}
echo >>${make_name}

for vertex in 1000 10000 100000
do
i=1
while [ `expr ${start_num} + '(' ${i} - 1 ')' '*' ${cd_num}` -le ${end_num} ]
do
input_view ${make_name} "image_ranking_zeroskip_${vertex}.eps" `expr ${start_num} + '(' ${i} - 1 ')' '*' ${cd_num}` ${view_size} ${input_num} "頂点数${vertex}の次数分布グラフ"
i=`expr ${i} + 1`
input_num=`expr ${input_num} + 1`
done
done

echo "END"
echo "ALL END"