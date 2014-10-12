//【1回、スケールフリーグラフを制作するプログラム】2013.01/06,14:56 Yuho.Tanaka
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<stdlib.h>
#include<sstream>
#include<time.h>

using namespace std;

struct vertex{
    int number;
    vector<vertex*>edge;
};

//プロトタイプ宣言
void make_scale(string file_name,int vertex_max);

/*コマンド引数で入力
 1:生成するファイルの名前
 2:最大頂点数
*/
int main(int argc,char *argv[]){
    srand((unsigned int)time(NULL));
    
    make_scale(argv[1],atoi(argv[2]));
    cout<<"finish graph "<<endl;

    return 0;
}

//ファイルに出力
void make_scale(string file_name,int vertex_max){
    
    vector<int> array;
    ofstream graph;
    
    //ファイルの読み込み
    graph.open(file_name.c_str(),ios::app);
    
    graph<<vertex_max<<" "<<vertex_max-1<<endl;
    array.push_back(0);
    array.push_back(1);
    graph<<0<<" "<<1<<endl;
    
    for(int i=1;i<vertex_max-1;i++){
        int t;
        t=rand()%(2*i);//問題箇所
        array.push_back(array[t]);
        array.push_back(i+1);
        graph<<array[t]<<" "<<i+1<<endl;
    }
    graph.close();
}