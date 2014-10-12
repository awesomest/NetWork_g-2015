//【グラフの直径、半径、平均値を測定する（関数ループ）プログラム】2013.01/019.01.22,Yuho.Tanaka
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<sstream>
#include<queue>

using namespace std;

struct vertex{
    int number;
    vector<vertex*>edge;
};

/* プロトタイプ宣言 */
bool read_vertex(string file_name,int &vertex_max);
void input(std::vector<vertex>&G,string file_name,int &vertex_max);
void measurement(std::vector<vertex>&G,int &vertex_max,int &max,int &min,int &total);
int eccentricity_input(std::vector<vertex>&G,int num,int &vertex_max);
void depth_search(std::vector<vertex>&G,int root,int node,int depth[],int &vertex_max,queue<int>&vertex_num);
void output(string file_name2,int &vertex_max,int &max,int &min,int &total,int file_number);

/*コマンド引数で入力
 1:読み取るファイルの名前
 2:保存するのファイルの名前
 3:保存するのファイルの番号
*/
int main(int argc,char *argv[]){
    int vertex_max;//最大頂点数
    int e_max,e_min,e_total;
    
    if(read_vertex(argv[1],vertex_max)){//中身がtrueの場合実行
        vector<vertex>G(vertex_max);//vertex_maxの値を一つ上のif文でを修得する為、ここで宣言
        input(G,argv[1],vertex_max);
        measurement(G,vertex_max,e_max,e_min,e_total);
        output(argv[2],vertex_max,e_max,e_min,e_total,atoi(argv[3]));
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
//測定（直径、半径、平均値）
void measurement(std::vector<vertex>&G,int &vertex_max,int &max,int &min,int &total){
    vector<int>eccentricity;
    total=0;
    double average;
    
    for(int i=0;i<vertex_max;i++){
        //各頂点のEccentricityの測定
        eccentricity.push_back(eccentricity_input(G,i,vertex_max));
        //全頂点のEccentricityの合計
        total=total+eccentricity[i];
        //初期化
        if(i==0)max=min=eccentricity[i];
        //全頂点中のEccentricityの最大＆最小
        else{
            if(max<eccentricity[i])max=eccentricity[i];
            if(min>eccentricity[i])min=eccentricity[i];
        }
    }
}
//Eccentricityの測定
int eccentricity_input(std::vector<vertex>&G,int num,int &vertex_max){
    queue<int>vertex_num;
    int depth[vertex_max];
    int max=0;
    
    //初期化
    depth[num]=0;
    for(int i=0;i<vertex_max;i++){
        if(i!=num)depth[i]=vertex_max;
    }
    //numがたどり着く全深さをdepth[]へ格納
    depth_search(G,num,num,depth,vertex_max,vertex_num);
    
    //Eccentricityを測定
    for(int i=0;i<vertex_max;i++){
        if(max<depth[i])max=depth[i];
    }
    return max;
}
//nodeがたどり着く全深さを探索
void depth_search(std::vector<vertex>&G,int root,int node,int depth[],int &vertex_max,queue<int>&vertex_num){
    
    //頂点nodeの接続頂点から探索
    for(int i=0;i<(G[node].edge).size();i++){
        //木の一番上（根）にいる時
        if(vertex_num.empty()){
            //push
            vertex_num.push(G[node].edge[i]->number);
            //深さを格納
            depth[G[node].edge[i]->number]=1;
        }
        //木の途中にいる時
        else if(depth[G[node].edge[i]->number] > depth[node]){
            //push
            vertex_num.push(G[node].edge[i]->number);
            //深さを格納
            depth[G[node].edge[i]->number]=depth[node]+1;
        }
    }
    //pop
    if(root!=node)vertex_num.pop();
    //次の接続頂点へ移動
    if(!vertex_num.empty())depth_search(G,root,vertex_num.front(),depth,vertex_max,vertex_num);
}
//出力（直径、半径、平均値）
void output(string file_name2,int &vertex_max,int &max,int &min,int &total,int file_number){
    double average;
    average=1.0 * total / vertex_max;
    
    //表示
    cout<<"直径:"<<max<<endl;
    cout<<"半径:"<<min<<endl;
    cout<<"平均値:"<<average<<endl;
    
    //表示結果をファイルに格納
    ofstream graph;
    graph.open(file_name2.c_str(),ios::app);
    if(file_number==1){
        graph<<"#【各グラフの直径・半径・平均値】"<<endl;
        graph<<"#グラフの番号  "<<"直径  "<<"半径  "<<"平均値"<<endl;
    }
    graph<<file_number<<"   "<<max<<"   "<<min<<"   "<<average<<endl;
    
    graph.close();
}
