//【txtファイルからグラフが連結か非連結かを確認し非連結だった場合、連結成分とその個数を調べるプログラム（本年度未使用）】
//出力の際にmaxやcountを用いているため動作が重いかもしれない
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
void test_output_on_terminal(graph G);
void connect_group_count(graph G, vector<int> &connect_group);
void test_output_connect_group(graph G,vector<int>connect_group);
void output_connect_group(graph G,vector<int>connect_group);
/*コマンド引数で入力
 1:読み取るファイルの名前
*/
int main(int argc,char *argv[]){
    if(read_vertex(argv[1])){//中身がtrueの場合実行
	graph G;
	vector<int>connect_group;
        input_graph(G,argv[1]);
	//test_output_on_terminal(G);
	connect_group_count(G,connect_group);
	//test_output_connect_group(G,connect_group);
	output_connect_group(G,connect_group);
    }
    return 0;
}
//ファイルの読み込み
bool read_vertex(string file_name){
    ifstream graph;
    int node,edge;
    
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



void connect_group_count(graph G,vector<int>&connect_group){

    for(int i=0;i<G.vertex_num;i++) connect_group.push_back(0);

    set<int>visited;
    set<int>fringe;
    set<int>::iterator it;
    
    int group=1;
    int k=0;
    for(;k<G.vertex_num;k++){
	
	if(connect_group[k]==0){

	    fringe.insert(k);

	    while(!fringe.empty()){
		it = fringe.begin();
		int node=*it;
		for(int i=0;i<G.V[node].degree;i++){
		    if(visited.find(G.V[node].edge[i]->number)==visited.end()) fringe.insert(G.V[node].edge[i]->number);
	        }
		visited.insert(node);
		connect_group[node]=group;
		fringe.erase(node);

	    }
	    k=0;
	    group++;
	    visited.clear();
	    fringe.clear();
	}
    }
}


void test_output_on_terminal(graph G){
	cout<<"node:"<<G.vertex_num<<" edge:"<<G.edge_num<<endl;
	for(int i=0;i<G.vertex_num;i++){
		cout<<"頂点_"<<i<<"  次数_"<<G.V[i].degree<<" ";
		for(int j=0;j<G.V[i].degree;j++){
			cout<<G.V[i].edge[j]->number<<" ";
		}
		cout<<endl;
	}
}



void test_output_connect_group(graph G,vector<int>connect_group){
	for(int i=0;i<G.vertex_num;i++) cout<<connect_group[i]<<" ";
	cout<<endl;
}



void output_connect_group(graph G,vector<int>connect_group){
	vector<int>::iterator max_it=max_element( connect_group.begin(), connect_group.end() );
	
	if(*max_it==1) cout<<"connection"<<endl;
	else{
		cout<<"unconnection: group num = "<<*max_it<<endl;
		for(int i=1;i<=*max_it;i++){
			const int connect_num = static_cast<int>( count( connect_group.begin(), connect_group.end(), i ) );
			cout<<"group "<<i<<":"<<connect_num<<endl;
		}
	}
}




