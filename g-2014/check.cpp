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

using namespace std;

struct vertex{
	int number,degree;
	vector<vertex*>edge;
};

struct graph{
	int vertex_num,edge_num;
	vector<vertex>V;
};

//プロトタイプ宣言
bool read_vertex(string file_name,int number,string extension);
void input_graph(graph &G,string file_name,int number,string extension);
bool is_selfloop(string file_name,int number,string extension);
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
		if(read_vertex(argv[1],i,argv[2])){//中身がtrueの場合実行
			graph G;
			input_graph(G,argv[1],i,argv[2]);
			cout<<argv[1]<<"_"<<i<<"\t";
			if(is_selfloop(argv[1],i,argv[2])){
				cout<<"selfloop"<<"\t";
				count_self++;
			}
			else cout<<"nothing selfloop"<<"\t";
			if(is_multiple_edge(G)){
				cout<<"multiple edge"<<"\t";
				count_multiple++;
			}
			else cout<<"nothing multiple edge"<<"\t";
			if(is_connected(G)) cout<<"connection"<<endl;
			else{
				cout<<"unconnection"<<endl;//自分の確認用のif文
				count_connect++;
			}
		}
	}
	cout<<count_self/(float)atoi(argv[4])<<"\t"<<count_multiple/(float)atoi(argv[4])<<"\t"<<count_connect/(float)atoi(argv[4])<<endl;
	return 0;
}
//ファイルの読み込み
bool read_vertex(string file_name,int number,string extension){
	ifstream graph;
	int node,edge;

	graph.open(file_fullname(number,file_name,extension).c_str());
	//ファイルの読み込み（失敗）
	if(graph.fail()){
		cout<<"file open error"<<endl;
		return false;
	}

	graph.close();

	return true;
}
//ファイルを格納
void input_graph(graph &G,string file_name,int number,string extension){
	ifstream graph;
	int s,t,edge,node;

	//ファイルの読み込み
	graph.open(file_fullname(number,file_name,extension).c_str());
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
		graph>>t>>s;
		G.V[t].edge.push_back(&G.V[s]);
		G.V[s].edge.push_back(&G.V[t]);
		G.V[t].degree++;
		G.V[s].degree++;
	}
	graph.close();
}

//txtファイルからグラフが自己ループか判定するプログラム
bool is_selfloop(string file_name,int number,string extension){
	ifstream graph;
	int s,t,edge,node;

	//ファイルの読み込み
	graph.open(file_fullname(number,file_name,extension).c_str());
	graph>>node>>edge;

	for(int i=0;i<edge;i++){
		graph>>t>>s;
		// ここにもgraph.close()が必要
		if(t==s) return true;
	}
	graph.close();
	return false;
}

//構造体からグラフが多重辺を持つか判定する
bool is_multiple_edge(graph &G){
	for(int i=0;i<G.vertex_num;i++){
		// ソートして連続する頂点の有無を確認すればよい
		for(int j=0;j<G.V[i].degree;j++){
			for(int k=j+1;k<G.V[i].degree;k++){
				if(G.V[i].edge[j]->number == G.V[i].edge[k]->number) return true;
			}
		}
	}
	return false;
}

bool is_connected(graph &G){
	set<int>visited;
	set<int>fringe;
	set<int>::iterator it;

	fringe.insert(0);

	// setよりqueueが速い?=>queueがいい！
	while(!fringe.empty()){
		it = fringe.begin();
		int node=*it;
		for(int i=0;i<(G.V[node].edge).size();i++){
			if(visited.find(G.V[node].edge[i]->number)==visited.end()){
				fringe.insert(G.V[node].edge[i]->number);
			}
		}
		visited.insert(node);
		fringe.erase(node);

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

