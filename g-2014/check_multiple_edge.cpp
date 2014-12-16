//【txtファイルからグラフを読み込み組み多重辺か調べるプログラム】 
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
bool read_vertex(string file_name);
void input_graph(graph &G,string file_name);
bool is_multiple_edge(graph &G);

/*コマンド引数で入力
 1:読み取るファイルの名前
*/
int main(int argc,char *argv[]){
    srand((unsigned int)time(NULL));
    if(read_vertex(argv[1])){//中身がtrueの場合実行
	graph G;
        input_graph(G,argv[1]);
	if(is_multiple_edge(G)) cout<<"multiple edge"<<endl;
	else cout<<"nothing multiple edge"<<endl;
    }
    return 0;
}
//最大頂点数の読み込み
bool read_vertex(string file_name){
    ifstream graph;

    //ファイルの読み込み
    graph.open(file_name.c_str());
    //ファイルの読み込み（失敗）
    if(graph.fail()){
        cout<<"file open error"<<endl;
        return false;
    }

    graph.close();
    
    return true;
}
//ファイルを格納
void input_graph(graph &G,string file_name){
    ifstream graph;
    int s,t,edge,node;
    
    //ファイルの読み込み
    graph.open(file_name.c_str());
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

//構造体からグラフが多重辺を持つか判定する
bool is_multiple_edge(graph &G){
    for(int i=0;i<G.vertex_num;i++){
	for(int j=0;j<G.V[i].degree;j++){
	    for(int k=j+1;k<G.V[i].degree;k++){
		if(G.V[i].edge[j]->number == G.V[i].edge[k]->number) return true;
	    }
	}
    }
    return false;
}

