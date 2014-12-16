#!/bin/bash
#【実験プログラムの実行用】2013.01/15,18:18 Yuho.Tanaka

#実行ファイル名（コンパイル）
test=test_program.sh

#実験プログラムの実行
chmod 755 ${test}
while [ -n "$1" ];
do
    echo "${1%.*} start!!"
    ./${test} $1 ${1%.*} 1 300
    echo -e "\n"
    shift
done

echo -e "\nALL END"
