//次数分布をプロットする用のファイル作成
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <set>
#include <map>
#include <time.h>
#include <queue>
using namespace std;

struct vertex{
	int number,degree;
	multiset<int> edge;
};

struct graph{
	int vertex_num,edge_num;
	vector<vertex> V;
};

//ファイルを指定
string file_fullname(int number,string file_name,string extension){
	ostringstream ost;
	string file_number;
	ost << number;
	file_number = ost.str();
	return file_name+"_"+file_number+"."+extension;
}

//ファイルの読み込み
bool read_vertex(graph &G, string file_name, int number, string extension){
	ifstream graph;

	graph.open(file_fullname(number,file_name,extension).c_str());
	//ファイルの読み込み（失敗）
	if(graph.fail()){
		cout<<"file open error"<<endl;
		return false;
	}

	//ファイルの読み込み
	//graph.open(file_fullname(number,file_name,extension).c_str());
	int node,edge;
	graph>>node>>edge;
	G.vertex_num=node;
	G.edge_num=edge;
	for(int i=0;i<node;i++){
		vertex a;
		a.number=i;
		a.degree=0;
		G.V.push_back(a);
	}

	//ファイルの格納(頂点：頂点からの接続先)
	for(int i=0;i<edge;i++){
		int t, s;
		graph>>t>>s;
		G.V[t].edge.insert(s);
		G.V[s].edge.insert(t);
		G.V[t].degree++;
		G.V[s].degree++;
	}
	graph.close();

	return true;
}

bool degree_distributions(graph& G, string file_name, int number, string extension){

	vector<int> degree_count(G.vertex_num);
	for(int i = 0; i < degree_count.size(); i++){
		degree_count[i] = 0;
	}

	for(int i = 0; i < G.vertex_num; i++) degree_count[G.V[i].edge.size()]++;

	ofstream graph;
	graph.open(file_fullname(number, file_name+"_DDplot", extension).c_str());
	//ファイルの読み込み（失敗）
	if(graph.fail()){
		cout<<"file open error"<<endl;
		return false;
	}

	for(int i = 0; i < degree_count.size(); i++){
		if(degree_count[i] != 0){
			graph << i << " " << degree_count[i]/(G.edge_num*2.0) << endl;
		}
	}
	graph.close();

	return true;
}

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

			if(degree_distributions(G,argv[1],i,argv[2])){
				cout << "OK" << endl;
			}else{
				cout << "NG" << endl;
			}
		}
	}
}