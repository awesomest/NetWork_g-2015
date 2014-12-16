//【txtファイルからグラフを読み込みグラフを組み替えるプログラム】
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<sstream>
//#include<time.h>
#include <ctime>
#include <cstdlib>


using namespace std;

struct vertex{
    int number,degree;
    vector<vertex*>edge;
};
struct graph{
    int vertex_num,edge_num;
    vector<vertex>V;
};

class Random
{
public:
	// コンストラクタ
	Random()
	{
		srand( static_cast<unsigned int>( time(NULL) ) );
	}

	// 関数オブジェクト
	unsigned int operator()(unsigned int max)
	{
		double tmp = static_cast<double>( rand() ) / static_cast<double>( RAND_MAX );
		return static_cast<unsigned int>( tmp * max );
	}
};


//プロトタイプ宣言
bool read_vertex(string file_name);
void input_graph(graph &G,string file_name);
void graph_shuffle(graph &G,graph &G_r,Random r);
//int my_rand(int x);//return 0〜(x-1)
void test_output_on_terminal(graph G);
void output_txt_file(int number,string file_name,string extension, graph G);
string file_fullname(int number,string file_name,string extension);

/*コマンド引数で入力
 1:読み取るファイルの名前
 2:出力ファイル名のテンプレ
 3:出力ファイル名の拡張子
 4:start number
 5:end number
*/
int main(int argc,char *argv[]){
    if(read_vertex(argv[1])){//中身がtrueの場合実行
	graph G;
	Random r;
        input_graph(G,argv[1]);
	//test_output_on_terminal(G);
	for(int i=atoi(argv[4]);i<=atoi(argv[5]);i++){
	    graph G_r;
	    graph_shuffle(G,G_r,r);
	    //test_output_on_terminal(G_r);
	    output_txt_file(i,argv[2],argv[3],G_r);
	}
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

//graph G を組み替える関数
void graph_shuffle(graph &G,graph &G_r, Random r){

    vector<int> node_degree;
    G_r.vertex_num=G.vertex_num;
    G_r.edge_num=G.edge_num;

    for(int i=0;i<G.vertex_num;i++){
	for(int j=0;j<G.V[i].degree;j++) node_degree.push_back(i);
	vertex a;
	a.number=i;
	a.degree=G.V[i].degree;
	G_r.V.push_back(a);
    }

    random_shuffle( node_degree.begin(), node_degree.end(), r );

    for(int i=0;i<node_degree.size();i+=2){
	int s=node_degree[i],t=node_degree[i+1];
	G_r.V[s].edge.push_back(&G_r.V[t]);
	G_r.V[t].edge.push_back(&G_r.V[s]);
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

void output_txt_file(int number,string file_name,string extension,graph G){

	ofstream graph;
	graph.open(file_fullname(number,file_name,extension).c_str());

	graph<<G.vertex_num<<" "<<G.edge_num<<endl;

	for(int i=0;i<G.vertex_num;i++){
		for(int j=0;j<G.V[i].degree;j++){
			int node=G.V[i].edge[j]->number;
			if(i<=node) graph<<i<<" "<<node<<endl;
			if(i==node) j++;
		}
	}
	graph.close();
}

/*int my_rand(int x){
   return (int)(((double)(rand())/(RAND_MAX+1.0))*(x));
}*/

string file_fullname(int number,string file_name,string extension){
    ostringstream ost;
    string file_number;
    ost << number;
    file_number = ost.str();
    return file_name+"_"+file_number+"."+extension;
}




