//【txtファイルからクラスター係数を計算するプログラム】
#include "../hpp/init.hpp"
#include "../hpp/file_operation.hpp"
#include "../hpp/check_feature.hpp"

/*コマンド引数で入力
1:読み取るファイルのテンプレ
2:読み取るファイルの拡張子
3:作成ファイル数の始まり
4:作成ファイル数の終わり
 */
int main(int argc, char *argv[]){
	for(int i = atoi(argv[3]); i <= atoi(argv[4]); i++){
		graph G;
		if(read_vertex(G,argv[1],i,argv[2])){//中身がtrueの場合実行
			cout<<argv[1]<<"_"<<i<<"\t";

			cout << clasta(G) <<  endl;
		}
	}
}
