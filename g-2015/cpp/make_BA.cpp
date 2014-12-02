//【指定したファイル数、BAモデルでグラフを制作するプログラム】
#include "../init.hpp"
#include "../file_operation.hpp"
#include "../make_network.hpp"

/*コマンド引数で入力
1:完全グラフの頂点数
2:追加する頂点数
3:新しい頂点の枝数
4:作成ファイル数の始まり
5:作成ファイル数の終わり
6:出力ファイル名のテンプレ
7:出力ファイル名の拡張子
 */

int main(int argc,char *argv[]){
	srand((unsigned int)time(NULL));

	for(int i=atoi(argv[4]);i<=atoi(argv[5]);i++){
		make_BA(i, atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), argv[6], argv[7]);
		cout<<"finish graph "<<i<<endl;
	}
	return 0;
}

