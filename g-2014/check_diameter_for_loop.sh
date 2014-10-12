#!/bin/bash
#【生成したグラフ（もとグラフ）の直径・半径・平均値を計算し出力をするためのプログラム（もとのネットワークを調べる）】2014 年01月22日 11時54分21秒
#$1 出力ファイルのテンプレ
#$3 ファイルの始まり
#$4 ファイルの終わり

R  --quiet --vanilla <<EOF
library(igraph)
for (i in $2:$3) {
file.name <- sprintf("${1%.*}_%d_check_main.sh", i)
out <- file(file.name, "w")
writeLines("# 直径 半径 平均値", out, sep="\n")
file.name <- sprintf("${1%.*}_%d.txt",i) 
file <- file.exists(file.name)
if(file){
gr <- matrix(scan(file.name,skip=1)+1,ncol=2, byrow=TRUE)
g <- graph.edgelist(gr, directed=FALSE)
eccentricity <- eccentricity(g)
writeLines(paste("n="), out, sep="")
writeLines(paste(i), out, sep=" ")
writeLines(paste("d="), out, sep="")
writeLines(paste(max(eccentricity)), out, sep=" ")
writeLines(paste("r="), out, sep="")
writeLines(paste(min(eccentricity)), out,sep=" ")
writeLines(paste("a="), out, sep="")
writeLines(paste(mean(eccentricity)), out,sep="\n")
}
}
EOF
