//【指定したファイル数、BAモデルでグラフを制作するプログラム】
#include "hpp/init.hpp"
#include "hpp/file_operation.hpp"
#include "hpp/make_network.hpp"

/*コマンド引数で入力
1:グラフの頂点数
2:作成ファイル数の始まり
3:作成ファイル数の終わり
4:出力ファイル名のテンプレ
5:出力ファイル名の拡張子
 */

int main(int argc,char *argv[]){
	srand((unsigned int)time(NULL));

	for(int i=atoi(argv[2]);i<=atoi(argv[3]);i++){
		make_random_tree(i, atoi(argv[1]), argv[4], argv[5]);
		cout<<"finish graph "<<i<<endl;
	}
	return 0;
}

