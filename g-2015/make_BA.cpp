//【指定したファイル数、BAモデルでグラフを制作するプログラム】
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<sstream>
#include<time.h>
#include<set>

using namespace std;

//プロトタイプ宣言
void make_BA(int num, int complete_graph_vertex_num, int add_vertex_num, int m, string file_name, string extension);
int my_rand(int x);//return 0〜(x-1)
string file_fullname(int number,string file_name,string extension);

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

//ネットワーク作成
void make_BA(int 		num_file,
					 	 int 		complete_graph_vertex_num,
					 	 int 		add_vertex_num,
					 	 int 		m,
					 	 string file_name,
					 	 string extension) {

	ofstream graph;
	graph.open(file_fullname(num_file,file_name,extension).c_str());
	int num_v = complete_graph_vertex_num+add_vertex_num,
			num_e = (complete_graph_vertex_num*(complete_graph_vertex_num-1))/2 + add_vertex_num*m;
	graph << num_v << " " << num_e << endl;

	vector<int> m_vertices;
	set<int> selected_vertices;

	for(int i=0;i<complete_graph_vertex_num;i++){//完全グラフの作成
		for(int j=i+1;j<complete_graph_vertex_num;j++){
			m_vertices.push_back(i);
			m_vertices.push_back(j);
			graph << i << " " << j << endl;
		}
	}

	for(int i=0;i<add_vertex_num;i++){//追加する頂点のループ
		int number=i+complete_graph_vertex_num;

		while(selected_vertices.size()<m){//追加する辺のループ
			int t =my_rand(m_vertices.size());
			if(!selected_vertices.count(m_vertices[t])){//選ばれていなかった
				selected_vertices.insert(m_vertices[t]);
			}
		}
		set<int>::iterator it = selected_vertices.begin();
		for ( ; it != selected_vertices.end(); it++) {
			m_vertices.push_back(*it);
			m_vertices.push_back(number);
			graph << *it << " " << number << endl; 
		}
		selected_vertices.clear();
	}

	graph.close();
}


int my_rand(int x){
	return (int)(((double)(rand())/(RAND_MAX+1.0))*(x));
}

string file_fullname(int number,string file_name,string extension){
	ostringstream ost;
	string file_number;
	ost << number;
	file_number = ost.str();
	return file_name+"_"+file_number+"."+extension;
}
