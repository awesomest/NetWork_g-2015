//【txtファイルから自己ループ・多重辺・非連結かをまとめて確認するプログラム】
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<sstream>
#include<set>
#include<map>
#include<time.h>
#include<queue>

using namespace std;

struct vertex{
	int number,degree;
	multiset<int> edge;
};

struct graph{
	int vertex_num,edge_num;
	vector<vertex> V;
};

//プロトタイプ宣言
bool read_vertex(graph &G,string file_name,int number,string extension);
bool is_selfloop(graph &G);
bool is_multiple_edge(graph &G);
bool is_connected(graph &G);
string file_fullname(int number,string file_name,string extension);

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

//txtファイルからグラフが自己ループか判定するプログラム
bool is_selfloop(graph &G){
	for(int i=0;i<G.V.size();i++){
		if (G.V[i].edge.count(i)) return true;
	}
	return false;
}

//構造体からグラフが多重辺を持つか判定する
bool is_multiple_edge(graph &G){
	for(int i=0;i<G.vertex_num;i++){
		if (G.V[i].edge.empty()) continue;
		set<int>::iterator it = G.V[i].edge.begin();
		while (1) {
			// 頂点数１のときここでエラー？
			set<int>::iterator it_prev = it++;
			if (it == G.V[i].edge.end()) break;
			if (*it_prev == *it) return true;
		}
	}
	return false;
}

bool is_connected(graph &G){
	set<int>visited;
	queue<int> fringe;
	set<int>::iterator it;

	fringe.push(0);
	visited.insert(0);

	while(!fringe.empty()){
		int pos = fringe.front();
		fringe.pop();

		set<int>::iterator it = G.V[pos].edge.begin();
		while (it != G.V[pos].edge.end()) {
			if (!visited.count(*it)){
				fringe.push(*it);
				visited.insert(*it);
			}
			it++;
		}
	}

	if(visited.size()==G.vertex_num) return true;
	else return false;
}

//ファイルを指定
string file_fullname(int number,string file_name,string extension){
	ostringstream ost;
	string file_number;
	ost << number;
	file_number = ost.str();
	return file_name+"_"+file_number+"."+extension;
}

