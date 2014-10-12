//【次数を測るプログラム】2013.01/18,00.55 Yuho.Tanaka
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<sstream>

using namespace std;

struct vertex{
    int number;
    vector<vertex*>edge;
};

//プロトタイプ宣言
bool read_vertex(string file_name,int &vertex_max);
void input(std::vector<vertex>&G,string file_name,int &vertex_max);
void output(std::vector<vertex>&G,string file_name2,int &vertex_max);
void output2(std::vector<vertex>&G,string file_name3,int &vertex_max);

/*コマンド引数で入力
 1:読み取るファイルの名前
 2:保存するのファイルの名前
 3:保存するのファイルの名前(可視化用)
*/
int main(int argc,char *argv[]){
    int vertex_max;//最大頂点数
    if(read_vertex(argv[1],vertex_max)){//中身がtrueの場合実行
        vector<vertex>G(vertex_max);//vertex_maxの値を一つ上のif文でを修得する為、ここで宣言
        input(G,argv[1],vertex_max);
        output(G,argv[2],vertex_max);
        output2(G,argv[3],vertex_max);
    }
    return 0;
}
//最大頂点数の読み込み
bool read_vertex(string file_name,int &vertex_max){
    ifstream graph;
    int s,t;
    
    //ファイルの読み込み
    graph.open(file_name.c_str());
    //ファイルの読み込み（失敗）
    if(graph.fail()){
        cout<<"file open error"<<endl;
        return false;
    }
    //最大頂点数の入力
    graph>>t;
    vertex_max=t;
    
    graph.close();
    
    return true;
}
//ファイルを格納
void input(std::vector<vertex>&G,string file_name,int &vertex_max){
    ifstream graph;
    int s,t;
    
    //ファイルの読み込み
    graph.open(file_name.c_str());
    graph>>t>>s;

    //ファイルの格納(頂点：頂点からの接続先)
    for(int i=0;i<vertex_max;i++){
        graph>>t>>s;
        G[i].number=i;
        G[t].edge.push_back(&G[s]);
        G[s].edge.push_back(&G[t]);
    }
    G[t].edge.pop_back();
    G[s].edge.pop_back();

    graph.close();
}
//出力（次数の表示）
void output(std::vector<vertex>&G,string file_name2,int &vertex_max){
    //グラフに格納
    ofstream graph;
    graph.open(file_name2.c_str(),ios::app);
    graph<<"#【各頂点が接続している頂点とその次数】"<<endl;
    graph<<"#各頂点  各頂点の次数｜各頂点から接続している頂点"<<endl;
    for(int i=0;i<vertex_max;i++){
        graph<<"頂点_"<<i<<"  次数_"<<(G[i].edge).size()<<" : ";
        for(int j=0;j<(G[i].edge).size();j++){
            graph<<G[i].edge[j]->number<<" ";
        }
        graph<<endl;
    }
}
//出力（次数の表示⇒可視化用）
void output2(std::vector<vertex>&G,string file_name3,int &vertex_max){
    //グラフに格納(可視化用)
    ofstream graph;
    graph.open(file_name3.c_str(),ios::app);
    graph<<"id,name,links"<<endl;
    for(int i=0;i<vertex_max;i++){
        graph<<i<<","<<"["<<i<<"]";
        for(int j=0;j<(G[i].edge).size();j++){
            if(i<G[i].edge[j]->number)graph<<","<<G[i].edge[j]->number;
        }
        graph<<endl;
    }
}
