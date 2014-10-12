//【次数分布を格納】2013.01/13,09:48,Yuho.Tanaka
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
void plot_date(std::vector<vertex>&G,int &vertex_max,int degree_count[]);
void output(std::vector<vertex>&G,string file_name2,int &vertex_max,int degree_count[]);

/*コマンド引数で入力
 1:読み取るファイルの名前
 2:保存するのファイルの名前
*/
int main(int argc,char *argv[]){
    int vertex_max;//最大頂点数
    if(read_vertex(argv[1],vertex_max)){//中身がtrueの場合実行
        vector<vertex>G(vertex_max);//vertex_maxの値を一つ上のif文でを修得する為、ここで宣言
        int degree_count[vertex_max];//次数が選ばれた数
        input(G,argv[1],vertex_max);
        plot_date(G,vertex_max,degree_count);
        output(G,argv[2],vertex_max,degree_count);
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
    graph>>t>>s;
    vertex_max=t;
    
    graph.close();
    
    return true;
}
//ファイルの格納
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
//プロットするデータを格納
void plot_date(std::vector<vertex>&G,int &vertex_max,int degree_count[]){
    //初期化
    for(int i=1;i<vertex_max;i++)degree_count[i]=0;
    //次数の数を数える
    for(int i=0;i<vertex_max;i++)degree_count[(G[i].edge).size()]++;
}
//出力
void output(std::vector<vertex>&G,string file_name2,int &vertex_max,int degree_count[]){
    double k;
    
    //グラフに格納
    ofstream graph;
    graph.open(file_name2.c_str(),ios::app);
    graph<<"#【次数分布】"<<endl;
    graph<<"#次数ｋ  "<<"次数がｋである点の数  "<<"ｐ(ｋ)【次数がｋである点の数÷総次数】"<<endl;
    for(int i=1;i<vertex_max;i++){
        k=1.0 * degree_count[i] / vertex_max;
        graph<<i<<"   "<<degree_count[i]<<"   "<<k<<endl;
    }
    graph.close();
    cout<<"finish"<<endl;
}
