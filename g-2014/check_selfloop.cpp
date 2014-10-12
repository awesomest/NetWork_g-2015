//【txtファイルからグラフを読み込み自己ループの有無を確認するプログラム】
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

//プロトタイプ宣言
bool read_vertex(string file_name);
bool is_selfloop(string file_name);


/*コマンド引数で入力
 1:読み取るファイルの名前
*/
int main(int argc,char *argv[]){
    if(read_vertex(argv[1])){//中身がtrueの場合実行
        if(is_selfloop(argv[1])) cout<<"selfloop"<<endl;
	else cout<<"nothing selfloop"<<endl;
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
//txtファイルからグラフが自己ループか判定するプログラム
bool is_selfloop(string file_name){
    ifstream graph;
    int s,t,edge,node;
    
    //ファイルの読み込み
    graph.open(file_name.c_str());
    graph>>node>>edge;
    
    for(int i=0;i<edge;i++){
        graph>>t>>s;
        if(t==s) return true;
    }
    graph.close();
    return false;
}

