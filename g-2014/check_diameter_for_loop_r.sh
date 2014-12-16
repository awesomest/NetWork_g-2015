#!/bin/bash
#【生成したグラフ（組み替え）の直径・半径・平均値を計算し出力をするためのプログラム】2014 年01月22日 11時54分21秒 
#$1 出力ファイルのテンプレ
#$3 ファイルの始まり
#$4 ファイルの終わり
#$5 uncorrelate end

R  --quiet --vanilla <<EOF
library(igraph)
for (i in $2:$3) {
file.name <- sprintf("${1%.*}_%d_check.txt", i)
out <- file(file.name, "w")
writeLines("# 直径 半径 平均値", out, sep="\n")
for (j in 1:$4) {
file.name <- sprintf("${1%.*}_%d_uc_%d.txt", i,j) 
file <- file.exists(file.name)
if(file){ 
gr <- matrix(scan(file.name,skip=1)+1,ncol=2, byrow=TRUE)
g <- graph.edgelist(gr, directed=FALSE)
eccentricity <- eccentricity(g)
writeLines(paste(j), out, sep=" ")
writeLines(paste(max(eccentricity)), out, sep=" ")
writeLines(paste(min(eccentricity)), out,sep=" ")
writeLines(paste(mean(eccentricity)), out,sep="\n")
}
}
}
EOF
