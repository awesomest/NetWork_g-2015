//【指定したファイル数、修正BAモデルでネットワークを作成するプログラム】
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<sstream>
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
void make_graph_remakeBA(int vertex_num,graph &G);
int my_rand(int x);//return 0〜(x-1);
void output_txt_file(int number,string file_name,string extension, graph &G);
string file_fullname(int number,string file_name,string extension);

/*コマンド引数で入力
 1:生成するファイルのテンプレ
 2:生成するファイルの拡張子
 3:最大頂点数
 4:作成ファイル数の始まり
 5 作成ファイル数の終わり
*/
int main(int argc,char *argv[]){
    srand((unsigned int)time(NULL));
    
    for(int i=atoi(argv[4]);i<=atoi(argv[5]);i++){
	graph G;
        make_graph_remakeBA(atoi(argv[3]),G);
	output_txt_file(i,argv[1],argv[2],G);
        cout<<"finish graph "<<i<<endl;
    }
    return 0;
}

//ファイルに出力
void make_graph_remakeBA(int vertex_num,graph &G){
    
    vector<int> m_vertices;

    G.vertex_num=vertex_num;
    G.edge_num=vertex_num-1;

    for(int i=0;i<vertex_num;i++){
	vertex a;
	a.number=i;
	a.degree=0;
	G.V.push_back(a);
    }

    
    m_vertices.push_back(0);
    m_vertices.push_back(1);
    G.V[0].edge.push_back(&G.V[1]);
    G.V[1].edge.push_back(&G.V[0]);
    G.V[0].degree++;
    G.V[1].degree++;
    
    int t;
    
    for(int i=1;i<vertex_num-1;i++){ //i=1(?)
        t=my_rand(i*2);
        m_vertices.push_back(m_vertices[t]);
        m_vertices.push_back(i+1);
        G.V[m_vertices[t]].edge.push_back(&G.V[i+1]);
	G.V[i+1].edge.push_back(&G.V[m_vertices[t]]);
	G.V[m_vertices[t]].degree++;
	G.V[i+1].degree++;
    }
}

void output_txt_file(int number,string file_name,string extension,graph &G){

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
}

int my_rand(int x){
   return (int)(((double)(rand())/(RAND_MAX+1.0))*(x));
}

//ファイルを指定
string file_fullname(int number,string file_name,string extension){
    ostringstream ost;
    string file_number;
    ost << number;
    file_number = ost.str();
    return file_name+"_"+file_number+"."+extension;
}
