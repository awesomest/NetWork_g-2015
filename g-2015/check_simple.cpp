//【txtファイルから自己ループ・多重辺・非連結かをまとめて確認するプログラム】
#include "init.hpp"
#include "file_operation.hpp"
#include "check_feature.hpp"

/*コマンド引数で入力
1:読み取るファイルのテンプレ
2:読み取るファイルの拡張子
3:作成ファイル数の始まり
4:作成ファイル数の終わり
 */

int main(int argc,char *argv[]){
	float count_self=0,count_multiple=0,count_connect=0;
	for(int i=atoi(argv[3]);i<=atoi(argv[4]);i++){
		graph G;
		if(read_vertex(G,argv[1],i,argv[2])){//中身がtrueの場合実行
			cout<<argv[1]<<"_"<<i<<"\t";

			if(is_selfloop(G)){
				cout<<"selfloop"<<"\t";
				count_self++;
			} else cout<<"nothing selfloop"<<"\t";

			if(is_multiple_edge(G)){
				cout<<"multiple edge"<<"\t";
				count_multiple++;
			} else cout<<"nothing multiple edge"<<"\t";

			if(is_connected(G)) cout<<"connection"<<endl;
			else{
				cout<<"unconnection"<<endl;//自分の確認用のif文
				count_connect++;
			}
		}
	}
	cout<<count_self/(float)atoi(argv[4])<<"\t"<<count_multiple/(float)atoi(argv[4])<<"\t"<<count_connect/(float)atoi(argv[4])<<endl;
}
