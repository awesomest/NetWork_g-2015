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

struct vertex{
    int number,degree;
    vector<vertex*>edge;
};

struct graph{
    int vertex_num,edge_num;
    vector<vertex>V;
};


//プロトタイプ宣言
void make_BA(int complete_graph_vertex_num, int add_vertex_num,graph &G);
int my_rand(int x);//return 0〜(x-1)
void test_output_on_terminal(graph &G);
void output_txt_file(int number,string file_name,string extension, graph &G);
string file_fullname(int number,string file_name,string extension);

/*コマンド引数で入力
 1:完全グラフの頂点数
 2:追加する頂点数
 3:作成ファイル数の始まり
 4:作成ファイル数の終わり
 5:出力ファイル名のテンプレ
 6:出力ファイル名の拡張子
*/
int main(int argc,char *argv[]){
    srand((unsigned int)time(NULL));
    
    for(int i=atoi(argv[3]);i<=atoi(argv[4]);i++){
	graph G;
        make_BA(atoi(argv[1]),atoi(argv[2]),G);
	output_txt_file(i,argv[5],argv[6],G);//出力
	test_output_on_terminal(G);//確認用出力
        cout<<"finish graph "<<i<<endl;
    }
    return 0;
}

//ネットワーク作成
void make_BA(int complete_graph_vertex_num,int add_vertex_num,graph &G){
   
    vector<int> m_vertices;
    set<int> selected_vertices;

    for(int i=0;i<complete_graph_vertex_num+add_vertex_num;i++){
	vertex a;
	a.number=i;
	a.degree=0;
	G.V.push_back(a);
    }

    for(int i=0;i<complete_graph_vertex_num;i++){//完全グラフの作成
	for(int j=i+1;j<complete_graph_vertex_num;j++){
	    m_vertices.push_back(i);
	    m_vertices.push_back(j);
	    G.V[i].edge.push_back(&G.V[j]);
            G.V[j].edge.push_back(&G.V[i]);
	    G.V[i].degree++;
	    G.V[j].degree++;
	}
    }
    
    for(int i=0;i<add_vertex_num;i++){//追加する頂点のループ
			/*
			このmの計算はどういうこと？
			*/
	int m=my_rand(complete_graph_vertex_num/2)+complete_graph_vertex_num/2+1;//m0/2<=m<=m0
	int number=i+complete_graph_vertex_num;
	
        int add_edge=0;
	while(add_edge<m){//追加する辺のループ
            int t =my_rand(m_vertices.size());
	    if(find(selected_vertices.begin(),selected_vertices.end(),m_vertices[t])==selected_vertices.end() && m_vertices[t]!=number){//選ばれていなかった
	        m_vertices.push_back(m_vertices[t]);
                m_vertices.push_back(number);
		selected_vertices.insert(m_vertices[t]);
		add_edge++;
		G.V[m_vertices[t]].edge.push_back(&G.V[number]);
		G.V[number].edge.push_back(&G.V[m_vertices[t]]);
		G.V[m_vertices[t]].degree++;
		G.V[number].degree++;
	    }
	}
	selected_vertices.clear();
    }

    G.vertex_num=complete_graph_vertex_num+add_vertex_num;
    G.edge_num=m_vertices.size()/2;
}
//確認用.構造体に格納したグラフをターミナル上に表示
void test_output_on_terminal(graph &G){
	cout<<"node:"<<G.vertex_num<<" edge:"<<G.edge_num<<endl;
	for(int i=0;i<G.vertex_num;i++){
		cout<<"頂点_"<<i<<"  次数_"<<G.V[i].degree<<" ";
		for(int j=0;j<G.V[i].degree;j++){
			cout<<G.V[i].edge[j]->number<<" ";
		}
		cout<<endl;
	}
}

//構造体に格納したグラフを昨年までのtxt型にして出力
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
